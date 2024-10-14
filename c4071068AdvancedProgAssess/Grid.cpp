#include "Grid.hpp"
#include "Vector2.hpp"
#include "Pattern.hpp"
#include "Patterns.hpp"

#include <cstdlib>
#include <iostream> 
#include <windows.h>
#include <bitset>
#include <thread>
#include <vector>

using namespace std;

void Grid::ClearGrid()
{
	for (int i = 0; i < this->gridWidth; i++)
	{
		for (int j = 0; j < this->gridHeight; j++)
		{
			this->grid[i][j] = Cell();
		}
	}
}

void Grid::SetAliveCells(int aliveSquares, int randomSeed) 
{
	srand(randomSeed); 
	for (int i = 0; i < aliveSquares; i++) {

		Vector2<int> pos = Vector2<int>(
			rand() % this->gridWidth, 
			rand() % this->gridHeight
		);

		pos = pos + 10; // Using operator overloading
		GetNextFree(&pos, 1);
		this->grid[pos.x][pos.y].alive = true;
	}
}

// Set random value to next value accross if this value has already been selected 
// (do recursive where neccessary)
bool Grid::GetNextFree(Vector2<int>* pos, int stepCount)
{
	if (stepCount >= this->gridHeight * gridWidth)
		return false;

	if (pos->x >= this->gridWidth)
	{
		pos->x = 0;
		(pos->y)++;
	}

	if (pos->y >= this->gridHeight)
		pos->y = 0;
	
	if (this->grid[pos->x][pos->y].alive)
		return GetNextFree(pos, stepCount + 1);
	return true;
}

void Grid::PrintGrid() 
{
	for (int i = 0; i < this->gridWidth; i++)
	{
		cout << ".";
		for (int j = 0; j < this->gridHeight; j++)
		{
			if (this->grid[i][j].alive)
				cout << "o";
			else
				cout << " ";
			cout << ".";
		}
		cout << endl;
	}
}

// Pass neighbouring cell states to each cell such that it can work out its next state
void Grid::GetCellsInThreads(int* x) 
{
	bool* neighbours = new bool[8];
	for (int y = 0; y < this->gridHeight; y++)
	{
		int index = 0;

		for (int offsetX = -1; offsetX <= 1; offsetX++)
		{
			for (int offsetY = -1; offsetY <= 1; offsetY++)
			{
				if (offsetX == 0 && offsetY == 0)
					continue;

				int neighbourX = *x + offsetX;
				int neighbourY = y + offsetY;

				if (neighbourX >= 0 && neighbourX < gridWidth && neighbourY >= 0 && neighbourY < gridHeight)
					neighbours[index++] = this->grid[neighbourX][neighbourY].alive;
				else
					neighbours[index++] = false;
			}
		}
		this->grid[*x][y].GetNextState(neighbours, 8);
	}
	delete[] neighbours;
	//delete x;
}

// Calls GetCellsInThreads via threads for each row then join threads
void Grid::GetNextCells()
{
	std::vector<std::thread> threads;

	for (int x = 0; x < this->gridWidth; x++)
	{
		threads.emplace_back(&Grid::GetCellsInThreads, this, new int(x));  
	}

	for (int i = 0; i < threads.size(); ++i) {
		threads[i].join();
	}
}

void Grid::SetNextCells() 
{
	for (int i = 0; i < this->gridWidth; i++)
	{
		for (int j = 0; j < this->gridHeight; j++)
		{
			this->grid[i][j].SetNextState();
		}
	}
}


// NOTE TO SELF> Combine with default pattern check to avoid an insane amount of loops
bool Grid::IsGridEmpty() 
{
	for (int i = 0; i < this->gridWidth; i++)
	{
		for (int j = 0; j < this->gridHeight; j++)
		{
			if (this->grid[i][j].alive)
				return false;
		}
	}
	return true;
}


// Get grid area based on dimensions such that it can be comapred to pattern

// for 8 by 8 to 3 by 5
// 00000000
// 00000000
// 00000000
// 00000000
// 00111100
// 00111100
// 00000000
// 00000000


// 3 by 5 => 

// 000
// 000
// 000
// 000
// 001

template <size_t PatternSize, size_t GridSize>
std::bitset<PatternSize> Grid::ExtractPattern(int row, int col, int patternHeight, int patternWidth,
	int gridWidth, const std::bitset<GridSize>& gridSegment)
{
	std::bitset<PatternSize> extractedPattern;

	for (int j = 0; j < patternHeight; j++) 
	{
		for (int i = 0; i < patternWidth; i++) 
		{

			// Discovered via heavy debugging index of bitset seems to start from right of bitset data structure
			int gridIndex = GridSize - 1 - (((row + j) * gridWidth) + (col + i));
			int patternIndex = PatternSize - 1 - ((j * patternWidth) + i);

			if (gridIndex < GridSize)
				extractedPattern[patternIndex] = gridSegment[gridIndex];
			else
				extractedPattern[patternIndex] = 0;
		}
	}
	return extractedPattern;
}

// Check for overlap for each bit (including dead cells)
template <size_t PatternSize, size_t GridSize>
bool Grid::HasOverlap(const std::bitset<GridSize>& gridSegment, const std::bitset<PatternSize>& myPattern,
	int selectionWidth, Vector2<int> dimensions)
{
	int maxRow = selectionWidth;
	int maxCol = selectionWidth;

	for (int row = 0; row < maxRow; row++) {
		for (int col = 0; col < maxCol; col++) {

			// Get the pattern to compare: 64 by 64 has different dimensions to 4 by 4. 
			// We can't do a direct comparison as 0001100000011000 would be compared to 01100110 (due to borders)
			std::bitset<PatternSize> extractedPattern = ExtractPattern<PatternSize, 64>(row, col, 
				dimensions.y, dimensions.x, selectionWidth, gridSegment);

			if (extractedPattern == myPattern)
				return true;
		}
	}
	return false;
}

// Create a bit set pattern of a defined bit size that can be used to compare against any pattern
// dimesion is the width/height of the box selection- technically could get this from sqr(ptrSize) but that would be expensive
template <size_t PatternSize>
std::bitset<PatternSize> Grid::GridGetBoxSelection(Vector2<int> const pos, int dimension)
{
	std::bitset<PatternSize> selection;
	for (int i = 0; i < dimension; i++) 
	{
		for (int j = 0; j < dimension; j++) 
		{
			int gridX = pos.x + i;
			int gridY = pos.y + j;

			if (gridX >= 0 && gridX < this->gridWidth && 
				gridY >= 0 && gridY < this->gridHeight)
				selection[(i * dimension) + j] = this->grid[gridX][gridY].alive ? 1 : 0;
			else
				selection[(i * dimension) + j] = 0;
		}
	}
	return selection;
}

template <size_t PatternSize> bool Grid::CheckForOverlap(PatternMask<PatternSize> patternBase, std::bitset<64> bits)
{
	const std::bitset<PatternSize> myPattern = patternBase.GetPattern();
	const Vector2<int> patternDimensions = patternBase.GetDimensions();

	if (HasOverlap<PatternSize, 64>(bits, myPattern, 8, patternDimensions)) 
		return true;
	return false;
}

// Checks a 8 by 8 grid around every alive block and compares it to each pattern
bool Grid::CheckForPattern(Pattern pattern) 
{
	for (int i = 0; i < this->gridWidth; i++)
	{
		for (int j = 0; j < this->gridHeight; j++)
		{
			if (this->grid[i][j].alive) 
			{

				Vector2<int> pos = Vector2<int>(i - 2, j - 2);
				std::bitset<64> bits = GridGetBoxSelection<64>(pos, 8);
				int aliveSquares = bits.count(); // use for optimizations 
				bool end;
				switch (pattern)
				{
					case Grid::Glider:
						end = CheckForOverlap<25>(Patterns::glider, bits);
						if (end)
							return end;
						break;
					case Grid::LWSS:
						end = CheckForOverlap<42>(Patterns::lwss, bits);
						if (end)
							return end;
						break;
					case Grid::Toad:
						end = CheckForOverlap<24>(Patterns::toadA, bits);
						if (end)
							return end;
						break;
					case Grid::Blinker:
						end = CheckForOverlap<15>(Patterns::blinkerA, bits);
						if (end)
							return end;
						break;
					case Grid::Beehive:
						end = CheckForOverlap<30>(Patterns::beehive, bits);
						// flipped
						if (end)
							return end;
						break;
					case Grid::Block:
						end = CheckForOverlap<16>(Patterns::block, bits);
						if (end)
							return end;
						break;
					default:
						break;
				}
			}
		}
	}

	return false;
}

bool Grid::WithInMaxSteps() {
	this->stepCount++;
	return this->stepCount < this->maxSteps;
}


UpdateResult Grid::UpdateGrid()
{
	GetNextCells();
	SetNextCells();

	if (IsGridEmpty())
		return GridEmpty;
	else if (!(WithInMaxSteps()))
		return StepCountExceeded;
	return Continue; 
}

UpdateResult Grid::UpdateGrid(Pattern endsOn)
{
	GetNextCells();
	SetNextCells();

	if (IsGridEmpty())
		return GridEmpty;
	else if (!(WithInMaxSteps()))
		return StepCountExceeded;
	else if (CheckForPattern(endsOn))
		return PatternFound;
	return Continue;
}

void Grid::TerminateGrid() {
	delete[] this->grid;
}

void Grid::SetUpGrid(int aliveSquares, int randomSeed)
{
	this->grid = new Cell* [this->gridWidth];

	for (int i = 0; i < this->gridHeight; i++) {
		grid[i] = new Cell [this->gridHeight];
	}

	ClearGrid();
	SetAliveCells(aliveSquares, randomSeed);

}