#include "stdafx.h"
#include "FieldCell.h"

using namespace icc::MineSweeper;

BOOL icc::MineSweeper::FieldCell::IsEmpty() const
{
	return countMinesAround == 0;
}

BOOL icc::MineSweeper::FieldCell::IsOpened() const
{
	return isOpened;
}

void icc::MineSweeper::FieldCell::SetAsOpened()
{
	isOpened = TRUE;
}

BOOL icc::MineSweeper::FieldCell::HasMine() const
{
	return hasMine;
}

void icc::MineSweeper::FieldCell::SetAsMined()
{
	hasMine = TRUE;
}

void icc::MineSweeper::FieldCell::AddNeighbors(FieldCell* n1, FieldCell* n2)
{
	allNeighbors.push_back(n1);
	allNeighbors.push_back(n2);
}

std::vector<FieldCell*>& icc::MineSweeper::FieldCell::GetNeighbors()
{
	return allNeighbors;
}

size_t icc::MineSweeper::FieldCell::GetCellIndex() const
{
	return cellIndex;
}

void icc::MineSweeper::FieldCell::AddNeighbors(FieldCell* n1, FieldCell* n2, FieldCell* n3)
{
	allNeighbors.push_back(n1);
	allNeighbors.push_back(n2);
	allNeighbors.push_back(n3);
}

icc::MineSweeper::FieldCell::FieldCell(size_t index) : hasMine(false), countMinesAround(0), isOpened(FALSE),
	cellIndex(index)
{

}

void icc::MineSweeper::FieldCell::CalcMinesAround()
{
	countMinesAround = 0;
	for (auto neighbor : allNeighbors)
	{
		if (neighbor->HasMine())
			++countMinesAround;
	}
}

size_t icc::MineSweeper::FieldCell::GetCountMinesAround()
{
	return countMinesAround;
}

