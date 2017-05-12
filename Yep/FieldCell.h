#pragma once

namespace icc
{
	namespace MineSweeper
	{
		class FieldCell
		{
		public:
			FieldCell(size_t cellIndex);
			virtual ~FieldCell() {}
			
			void	CalcMinesAround();
			size_t	GetCountMinesAround();

			BOOL	IsEmpty() const;

			BOOL	IsOpened() const;
			void	SetAsOpened();

			BOOL	HasMine() const;
			void	SetAsMined();

			//передаваемые параметры - соседи для клетки
			void	AddNeighbors(FieldCell* n1, FieldCell* n2, FieldCell* n3);
			void	AddNeighbors(FieldCell* n1, FieldCell* n2);

			std::vector<FieldCell*>&	GetNeighbors();

			size_t	GetCellIndex() const;
		protected:
		private:
			FieldCell(const FieldCell& other);
			FieldCell& operator=(const FieldCell& copy);
		private:
			size_t	cellIndex;

			BOOL	isOpened;
			BOOL	hasMine;

			int		countMinesAround;
			std::vector<FieldCell*>	allNeighbors;
		};
	}
}