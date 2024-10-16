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

		enum Pattern { Empty, Glider, LWSS, Toad, Blinker, Beehive, Block };


		const static bool IsSpaceship(Pattern pattern)
		{
				switch (pattern)
				{
					case Glider: return true;
					case LWSS: return true;
					default: return false;
			}
		}

		const static char* GetPatternName(Pattern pattern) 
		{
			switch (pattern) 
			{
				case Blinker: return "Blinker";
				case Block: return "Block";
				case Glider: return "Glider";
				case LWSS: return "LWSS";
				case Toad: return "Toad";
				case Beehive: return "Beehive";
				case Empty: return "Empty";
				default: return "Unknown Pattern";
			}
		}

		UpdateResult UpdateGrid(Pattern endsOn);
		UpdateResult UpdateGrid();
		bool IsGridEmpty();
		void TerminateGrid();
		void PrintGrid();
		bool WithInMaxSteps();


	private:
		template <size_t PatternSize> std::bitset<PatternSize> GridGetBoxSelection(Vector2<int> const coord, int dimension, int historyIndex);
		template <size_t PatternSize, size_t GridSize> bool HasOverlap(
			const std::bitset<GridSize>& gridSegment,
			const std::bitset<PatternSize>& myPattern,
			int selectionWidth, Vector2<int> dimensions);
		template <size_t PatternSize, size_t GridSize>std::bitset<PatternSize> ExtractPattern(
			int row, int col, int patternHeight, int patternWidth,
			int gridWidth, const std::bitset<GridSize>& gridSegment);

		template <size_t PatternSize> bool GetHistory(Vector2<int> pos, PatternMask<PatternSize> pattern);

		template <size_t PatternSize> bool CheckForOverlap(PatternMask<PatternSize> patternBase, std::bitset<64> bits);
		void ClearGrid();
		void GetCellsInThreads(int* x);
		bool GetNextFree(Vector2<int>* coord, int stepCount);
		void GetNextCells();
		void SetNextCells();
		void SetAliveCells(int aliveSquares, int randomSeed);
		bool CheckForPattern(Pattern pattern);
};

