#pragma once
#include "Cell.h"
#include <bitset>
#include "Vector2.h"

class Grid
{
	public:
		Cell** grid;
		int gridWidth;
		int gridHeight;

		int stepCount;
		int maxSteps;

		Grid(int gridWidth, int gridHeight, int aliveSquares, int maxSteps, int randomSeed)
		{
			this->gridWidth = gridHeight;
			this->gridHeight = gridHeight;
			this->maxSteps = maxSteps;
			SetUpGrid(aliveSquares, randomSeed);
		};

		void SetUpGrid(int aliveSquares, int randomSeed);
		enum Pattern { Empty, SpaceShip, Glider, LWSS, Toad, Blinker, Beehive, Block };

		bool UpdateGrid(Pattern endsOn);
		bool UpdateGrid();
		bool IsGridEmpty();
		void TerminateGrid();
		void PrintGrid();

	private:
		template <size_t PatternSize> std::bitset<PatternSize> GridGetBoxSelection(Vector2<int> const coord, int dimension);
		template <size_t PatternSize, size_t GridSize> bool HasOverlap(
			const std::bitset<GridSize>& gridSegment, 
			const std::bitset<PatternSize>& myPattern, 
			int gridWidth, int gridHeight, int patternWidth, int patternHeight);
		template <size_t PatternSize, size_t GridSize>std::bitset<PatternSize> ExtractPattern(
			int row, int col, int patternHeight, int patternWidth,
			int gridWidth, const std::bitset<GridSize>& gridSegment);

		void ClearGrid();
		void GetCellsInThreads(int* x);
		bool GetNextFree(Vector2<int>* coord, int stepCount);
		void GetNextCells();
		void SetNextCells();
		void SetAliveCells(int aliveSquares, int randomSeed);
		bool CheckForPattern(Pattern pattern);
		bool WithInMaxSteps();

		// bitwise comparator 
	
		// Get top square of moving block and do a block selection, get if left most pont has shifted from last frame with checking period 
		// SpaceShips go off the screen!!!!!!!! < maybe do a catch method with a buffer grid
		// ^^ Cannot be the only definitiopn of a space ship- spaceships need to be detecteed
		// before going off screen
		// 
		// 
		// Create an array of x size where each time a period is different extend the array size and check until its the same as first 
		// Recognition system is priority!!
};

