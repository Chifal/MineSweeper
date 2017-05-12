#pragma once

namespace icc
{
	namespace MineSweeper
	{
		class FieldCell
		{
		public:
			FieldCell();
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
		protected:
		private:
			FieldCell(const FieldCell& other);
			FieldCell& operator=(const FieldCell& copy);
		private:
			BOOL	isOpened;
			BOOL	hasMine;

			int		countMinesAround;
			std::vector<FieldCell*>	allNeighbors;
		};
	}
}