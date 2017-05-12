#pragma once


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

			void	CalcMines();
			void	GenerateMines();

			//������������ ��������� - ������� ������� ��� ������, ��� ������� � ������ ���-�� ��� ������
			inline	size_t	CalcMinesAround(size_t n1, size_t n2, size_t n3)
			{
				size_t result = 0;
				if (field[n1].hasMine)
					++result;
				if (field[n2].hasMine)
					++result;
				if (field[n3].hasMine)
					++result;

				return result;
			}
		private:
			struct FIELD_CELL
			{
				BOOL	isOpened;
				BOOL	hasMine;
				int		countMinesAround;
				FIELD_CELL() : hasMine(false), countMinesAround(0), isOpened(FALSE)
				{}
			};
			std::vector<FIELD_CELL>	field;

			size_t	width;
			size_t	height;
			size_t	countCells;
			size_t	countMines;

			//��� ������� ���� ������ ������ ������ ����������� �� ������� �� ����
			BOOL	isOpenedField;
		};
	}
}
