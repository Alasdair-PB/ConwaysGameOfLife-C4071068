#pragma once
#include "Cell.hpp"
#include <bitset>
#include "Vector2.hpp"
#include "Pattern.hpp"

enum UpdateResult { StepCountExceeded, Continue, PatternFound, GridEmpty };


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
		UpdateResult UpdateGrid(Pattern endsOn);
		UpdateResult UpdateGrid();
		bool IsGridEmpty();
		void TerminateGrid();
		void PrintGrid();
		bool WithInMaxSteps();


	private:
		template <size_t PatternSize> std::bitset<PatternSize> GridGetBoxSelection(Vector2<int> const coord, int dimension);
		template <size_t PatternSize, size_t GridSize> bool HasOverlap(
			const std::bitset<GridSize>& gridSegment,
			const std::bitset<PatternSize>& myPattern,
			int selectionWidth, Vector2<int> dimensions);
		template <size_t PatternSize, size_t GridSize>std::bitset<PatternSize> ExtractPattern(
			int row, int col, int patternHeight, int patternWidth,
			int gridWidth, const std::bitset<GridSize>& gridSegment);

		template <size_t PatternSize> bool CheckForOverlap(PatternMask<PatternSize> patternBase, std::bitset<64> bits);
		void ClearGrid();
		void GetCellsInThreads(int* x);
		bool GetNextFree(Vector2<int>* coord, int stepCount);
		void GetNextCells();
		void SetNextCells();
		void SetAliveCells(int aliveSquares, int randomSeed);
		bool CheckForPattern(Pattern pattern);

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

