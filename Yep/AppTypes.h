#pragma once

namespace icc
{
	namespace MineSweeper
	{
		static	const	size_t	DEFAULT_FIELD_WIDTH = 18;
		static	const	size_t	DEFAULT_FIELD_HEIGHT = 12;
		static	const	size_t	DEFAULT_COUNT_MINES = 60;

		static	const	LONG	DEFAULT_FIELD_CELL_SIZE = 47;
		
		struct Coord2x
		{
			size_t x;
			size_t y;
		};
		//TODO remove
		class PseudoRandom
		{
		public:
			PseudoRandom(uint64_t initValue = rand()) { prevValue = initValue; }
			virtual	uint64_t	Next()
			{
				//x = x * 48271 % 2147483647
				prevValue = prevValue * 16807 % 2147483647;
				return prevValue;
			}
			virtual	uint64_t	Next(uint32_t mod) { return Next() % mod; }
		private:
			uint64_t	prevValue;
		};

		enum class EOpenCellResult
		{
			Luckyboy,
			EndGame,
		};

		struct OPENED_CELL_INFO
		{
			const size_t	index;
			const size_t	countMinesAround;
			const BOOL		hasMine;

			OPENED_CELL_INFO(size_t indexValue, size_t countMinesAroundValue, BOOL hasMineValue) :
				index(indexValue), countMinesAround(countMinesAroundValue), hasMine(hasMineValue)
			{}
		};
	}
}