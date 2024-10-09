#include "Grid.h"
#include "Vector2.h"

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


// NOTE TO SELF> Make polymorhpic type for marks
namespace Patterns
{
	// Method for get Dimensions
	const std::bitset<16> block("0000011001100000");
}

// Get grid area based on dimensions such that it can be comapred to pattern
template <size_t PatternSize, size_t GridSize>
std::bitset<PatternSize> Grid::ExtractPattern(int row, int col, int patternHeight, int patternWidth,
	int gridWidth, const std::bitset<GridSize>& gridSegment)
{
	std::bitset<PatternSize> extractedPattern;

	for (int i = 0; i < patternHeight; i++) 
	{
		for (int j = 0; j < patternWidth; j++) 
		{
			int gridIndex = (row + i) * gridWidth + (col + j);
			extractedPattern[i * patternWidth + j] = gridSegment[gridIndex];
		}
	}
	return extractedPattern;
}

// Check for overlap between bits
template <size_t PatternSize, size_t GridSize>
bool Grid::HasOverlap(const std::bitset<GridSize>& gridSegment, const std::bitset<PatternSize>& myPattern,
	int gridWidth, int gridHeight, int patternWidth, int patternHeight)
{
	// Don't look for the pattern where it wouldn't fit
	int maxRow = gridHeight - patternHeight;
	int maxCol = gridWidth - patternWidth;

	for (int row = 0; row <= maxRow; row++) {
		for (int col = 0; col <= maxCol; col++) {

			// Get the pattern to compare: 64 by 64 has different dimensions to 4 by 4. 
			// We can't do a direct comparison as 0001100000011000 would be compared to 01100110 (due to borders)
			std::bitset<PatternSize> extractedPattern = ExtractPattern<PatternSize, GridSize>(row, col, 
				patternHeight, patternWidth, gridWidth, gridSegment);
			if (extractedPattern == myPattern)
				return true;
		}
	}
	return false;
}

// Create a bit set pattern of a defined bit size that can be used to compare against any pattern
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
				selection[i * dimension + j] = this->grid[gridX][gridY].alive ? 1 : 0;
			else
				selection[i * dimension + j] = 0;
		}
	}
	return selection;
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
				if (HasOverlap<16, 64>(bits, Patterns::block, 8, 8, 4, 4))
				{
					cout << endl << "Block found!!" << endl;
					cout << bits << endl << Patterns::block << endl;
					return true;
				}
			}
		}
	}

	switch (pattern)
	{
	case Grid::Empty:
		return IsGridEmpty();
		break;
	case Grid::SpaceShip:
		break;
	case Grid::Glider:
		break;
	case Grid::LWSS:
		break;
	case Grid::Toad:
		break;
	case Grid::Blinker:
		break;
	case Grid::Beehive:
		break;
	case Grid::Block:
		break;
	default:
		break;
	}
	return false;
}

bool Grid::WithInMaxSteps() {
	this->stepCount++;
	if (this->stepCount >= this->maxSteps)
		cout << "Exceeded step count" << endl;

	return this->stepCount < this->maxSteps;
}

bool Grid::UpdateGrid() 
{
	GetNextCells();
	SetNextCells();
	return (!CheckForPattern(Empty)) && WithInMaxSteps();
}

bool Grid::UpdateGrid(Pattern endsOn)
{
	GetNextCells();
	SetNextCells();
	return (!CheckForPattern(Empty)) && (!CheckForPattern(endsOn)) && WithInMaxSteps();
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