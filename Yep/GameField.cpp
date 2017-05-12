#include "stdafx.h"
#include "GameField.h"

using namespace icc::MineSweeper;
icc::MineSweeper::GameField::GameField(size_t widthValue, size_t heightValue, size_t countMinesValue) : 
	width(widthValue), height(heightValue), countCells(widthValue * heightValue),
	countMines(countMinesValue), isOpenedField(FALSE)
{
	CreateField();
}

void icc::MineSweeper::GameField::DeleteField()
{
	field.clear();
}

void icc::MineSweeper::GameField::CreateField()
{
	field.resize(countCells);
	for (size_t i = 0; i < countCells; ++i)
	{
		field[i].hasMine = FALSE;
		field[i].countMinesAround = 0;
	}

	isOpenedField = FALSE;
}

icc::MineSweeper::GameField::~GameField()
{
	DeleteField();
}

void icc::MineSweeper::GameField::GenerateMines()
{
	PseudoRandom drand(__rdtsc());
	for (size_t i = 0; i < countCells; ++i)
		field[i].hasMine = (drand.Next(5) == 2) ? TRUE : FALSE;
}

size_t icc::MineSweeper::GameField::GetWidth() const
{
	return width;
}

size_t icc::MineSweeper::GameField::GetHeight() const
{
	return height;
}

EOpenCellResult icc::MineSweeper::GameField::OpenCell(size_t cellIndex, std::vector<OPENED_CELL_INFO>& openedCells)
{
	openedCells.clear();

	if (isOpenedField)
	{
		if (field[cellIndex].hasMine)
		{
			for (size_t i = 0; i < countCells; ++i)
			{
				OPENED_CELL_INFO cellInfo(i, field[i].countMinesAround, field[i].hasMine);
				openedCells.push_back(cellInfo);
			}

			return EOpenCellResult::EndGame;
		}
	}
	else
	{
		isOpenedField = TRUE;
		GenerateMines();

		if (field[cellIndex].hasMine)
		{
			field[cellIndex].hasMine = FALSE;			
			//замена мины в запрашиваемых координатах на первое свободное место
			for (size_t i = 0; i < countCells; ++i)
			{
				if (field[i].hasMine)
					;
				else
				{
					field[i].hasMine = TRUE;
					break;
				}
			}
		}

		CalcMines();
	}

	field[cellIndex].isOpened = TRUE;

	for (size_t i = 0; i < countCells; ++i)
	{
		OPENED_CELL_INFO cellInfo(i, field[i].countMinesAround, field[i].hasMine);
		openedCells.push_back(cellInfo);
	}

	//OPENED_CELL_INFO cellInfo(cellIndex, field[cellIndex].countMinesAround, field[cellIndex].hasMine);
	//openedCells.push_back(cellInfo);

	return EOpenCellResult::Luckyboy;
}

void icc::MineSweeper::GameField::CalcMines()
{
	if (field.size() == 0)
		return;

	size_t countMines = 0;

	//угловые точки особые, исключительные заразы
	{
		//левая верхняя
		size_t cornerCellIndex = 0;
		field[0].countMinesAround = 
					CalcMinesAround(cornerCellIndex + 1, cornerCellIndex + width, cornerCellIndex + width + 1);

		cornerCellIndex = (height - 1) * width; 	//левая нижняя
		field[cornerCellIndex].countMinesAround =
					CalcMinesAround(cornerCellIndex + 1, cornerCellIndex - width, cornerCellIndex - width + 1);

		cornerCellIndex = width - 1;	//правая верхняя
		field[cornerCellIndex].countMinesAround = 
							CalcMinesAround(cornerCellIndex - 1, cornerCellIndex + width, cornerCellIndex + width - 1);

		cornerCellIndex = height * width - 1; 	//правая нижняя
		field[cornerCellIndex].countMinesAround = 
							CalcMinesAround(cornerCellIndex - 1, cornerCellIndex - width, cornerCellIndex - width - 1);
	}

	const auto offsetTL = width + 1;	//смещение вверх влево
	const auto offsetTR = width - 1;	//вверх вправо
	const auto offsetBL = width - 1;	//вниз влево
	const auto offsetBR = width + 1;	//вниз вправо

	//левая и правая линия
	{
		for (size_t y = 1; y < height - 1; ++y)
		{
			const auto left = y * width;
			size_t countMines = CalcMinesAround(left + 1, left + width, left - width);
			if (field[left - offsetTR].hasMine)
				++countMines;
			if (field[left + offsetBR].hasMine)
				++countMines;
			field[left].countMinesAround = countMines;
		}

		for (size_t y = 1; y < height - 1; ++y)
		{
			const auto right = y * width + width - 1;
			size_t countMines = CalcMinesAround(right - 1, right + width, right - width);
			if (field[right - offsetTL].hasMine)
				++countMines;
			if (field[right + offsetBL].hasMine)
				++countMines;
			field[right].countMinesAround = countMines;
		}
	}

	//верхняя линия и нижняя линия
	{
		for (size_t x = 1; x < width - 1; ++x)
		{
			size_t countMines = 0;
			if (field[x - 1].hasMine)
				++countMines;
			if (field[x + 1].hasMine)
				++countMines;
			for (size_t i = x + offsetBL; i <= x + offsetBR; ++i)
			{
				if (field[i].hasMine)
					++countMines;
			}
			field[x].countMinesAround = countMines;
		}

		for (size_t x = (height - 1) * width + 1; x < countCells - 1; ++x)
		{
			size_t countMines = 0;
			if (field[x - 1].hasMine)
				++countMines;
			if (field[x + 1].hasMine)
				++countMines;
			for (size_t i = x - offsetTL; i <= x - offsetTR; ++i)
			{
				if (field[i].hasMine)
					++countMines;
			}
			field[x].countMinesAround = countMines;
		}
	}

	//и все остальное в центре
	{
		const size_t countCellInMiddleRow = width - 2;
		for (size_t y = 1; y < height - 1; ++y)
		{
			auto index = y * width + 1;
			for (size_t x = 0; x < countCellInMiddleRow; ++x)
			{
				size_t countMines = 0;
				//3 верхних клетки
				for (size_t i = index - offsetTL; i <= index - offsetTR; ++i)
				{
					if (field[i].hasMine)
						++countMines;
				}
				//3 нижних клетки
				for (size_t i = index + offsetBL; i <= index + offsetBR; ++i)
				{
					if (field[i].hasMine)
						++countMines;
				}

				if (field[index - 1].hasMine)
					++countMines;
				if (field[index + 1].hasMine)
					++countMines;

				field[index].countMinesAround = countMines;
				++index;
			}
		}
	}
}
