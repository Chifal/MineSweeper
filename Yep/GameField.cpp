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
	for (auto cell : field)
		delete cell;
}

void icc::MineSweeper::GameField::CreateField()
{
	field.resize(countCells);
	for (size_t i = 0; i < countCells; ++i)
		field[i] = new FieldCell();

	SetupNeighbors();
	isOpenedField = FALSE;
}

void icc::MineSweeper::GameField::SetupNeighbors()
{
	const auto offsetTL = width + 1;	//смещение вверх влево
	const auto offsetTR = width - 1;	//вверх вправо
	const auto offsetBL = width - 1;	//вниз влево
	const auto offsetBR = width + 1;	//вниз вправо

	//у угловых клеток только 3 соседа
	{
		size_t cornerCellIndex = 0;	//лева€ верхн€€
		field[cornerCellIndex]->AddNeighbors(field[cornerCellIndex + 1], 
											field[cornerCellIndex + width], field[cornerCellIndex + offsetBR]);

		cornerCellIndex = (height - 1) * width; 	//лева€ нижн€€
		field[cornerCellIndex]->AddNeighbors(field[cornerCellIndex + 1],
											field[cornerCellIndex - width], field[cornerCellIndex - offsetTR]);

		cornerCellIndex = width - 1;	//права€ верхн€€
		field[cornerCellIndex]->AddNeighbors(field[cornerCellIndex - 1],
											field[cornerCellIndex + width], field[cornerCellIndex + offsetBL]);

		cornerCellIndex = height * width - 1; 	//права€ нижн€€
		field[cornerCellIndex]->AddNeighbors(field[cornerCellIndex - 1],
											field[cornerCellIndex - width], field[cornerCellIndex - offsetTL]);
	}

	//лева€ и права€ вертикаль
	{
		for (size_t y = 1; y < height - 1; ++y)
		{
			const auto left = y * width;
			field[left]->AddNeighbors(field[left + 1], field[left + width], field[left - width]);
			field[left]->AddNeighbors(field[left - offsetTR], field[left + offsetBR]);

			const auto right = left + width - 1;
			field[right]->AddNeighbors(field[right - 1], field[right + width], field[right - width]);
			field[right]->AddNeighbors(field[right + offsetBL], field[right - offsetTL]);
		}
	}

	//верхн€€ лини€ и нижн€€ лини€
	{
		for (size_t x = 1; x < width - 1; ++x)
		{
			field[x]->AddNeighbors(field[x + offsetBL], field[x + offsetBL + 1], field[x + offsetBL + 2]);
			field[x]->AddNeighbors(field[x - 1], field[x + 1]);
		}

		for (size_t x = (height - 1) * width + 1; x < countCells - 1; ++x)
		{
			field[x]->AddNeighbors(field[x - offsetTL], field[x - offsetTL + 1], field[x - offsetTL + 2]);
			field[x]->AddNeighbors(field[x - 1], field[x + 1]);
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
				//3 верхних клетки
				field[index]->AddNeighbors(field[index - offsetTL], field[index - offsetTL + 1], field[index - offsetTL + 2]);

				//3 нижних клетки
				field[index]->AddNeighbors(field[index + offsetBL], field[index + offsetBL + 1], field[index + offsetBL + 2]);

				//и по бокам
				field[index]->AddNeighbors(field[index - 1], field[index + 1]);

				++index;
			}
		}
	}
}

icc::MineSweeper::GameField::~GameField()
{
	DeleteField();
}

void icc::MineSweeper::GameField::GenerateMines(size_t excludeCellIndex)
{
	PseudoRandom drand(__rdtsc());
	for (size_t i = 0; i < countCells; ++i)
	{
		if (i != excludeCellIndex && (drand.Next(5) == 2))
			field[i]->SetAsMined();
	}
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

	if (!isOpenedField)
	{
		isOpenedField = TRUE;
		GenerateMines(cellIndex);
		
		for (auto cell : field)
			cell->CalcMinesAround();
	}

	if (field[cellIndex]->HasMine())
	{
		for (size_t i = 0; i < countCells; ++i)
		{
			OPENED_CELL_INFO cellInfo(i, field[i]->GetCountMinesAround(), field[i]->HasMine());
			openedCells.push_back(cellInfo);
		}

		return EOpenCellResult::EndGame;
	}
	else
	{
		field[cellIndex]->SetAsOpened();

		if (field[cellIndex]->IsEmpty())
			OpenCellsAround(cellIndex, openedCells);
		else
		{
			OPENED_CELL_INFO cellInfo(cellIndex, field[cellIndex]->GetCountMinesAround(), field[cellIndex]->HasMine());
			openedCells.push_back(cellInfo);
		}

		return EOpenCellResult::Luckyboy;
	}
	//for (size_t i = 0; i < countCells; ++i)
	//{
	//	OPENED_CELL_INFO cellInfo(i, field[i].countMinesAround, field[i]->HasMine());
	//	openedCells.push_back(cellInfo);
	//}
}

void icc::MineSweeper::GameField::OpenCellsAround(size_t cellIndex, std::vector<OPENED_CELL_INFO>& openedCells)
{

}