#pragma once

#include "FieldCell.h"

namespace icc
{
	namespace MineSweeper
	{
		class GameField
		{
		public:
			GameField(size_t width, size_t height, size_t countMines);
			virtual	~GameField();

			size_t	GetWidth() const;
			size_t	GetHeight() const;

			//�������� ������ ������� �������� ������ � ���������� ������� ������� ������
			EOpenCellResult	OpenCell(size_t cellIndex, std::vector<OPENED_CELL_INFO>& openedCells);
		protected:
		private:
			GameField(const GameField& other);
			GameField& operator=(const GameField& copy);

			void	DeleteField();
			void	CreateField();

			void	SetupNeighbors();

			//������������ �������� - ������ � ������� �� ������ ���� ����
			void	GenerateMines(size_t excludeCellIndex);

			void	OpenCellsAround(size_t cellIndex, std::vector<OPENED_CELL_INFO>& openedCells);
			inline	size_t	CalcMinesAround(size_t n1, size_t n2, size_t n3)
			{
				size_t result = 0;
				if (field[n1]->HasMine())
					++result;
				if (field[n2]->HasMine())
					++result;
				if (field[n3]->HasMine())
					++result;

				return result;
			}
		private:
			std::vector<FieldCell*>	field;

			size_t	width;
			size_t	height;
			size_t	countCells;
			size_t	countMines;

			//��� ������� ���� ������ ������ ������ ����������� �� ������� �� ����
			BOOL	isOpenedField;
		};
	}
}
