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

		pos = pos + 1; // Using operator overloading
		pos = pos - 1; // Using operator overloading

		GetNextFree(&pos, 1);
		this->grid[pos.x][pos.y].alive = true;
	}
}

bool Grid::GetNextFree(Vector2<int>* pos, int stepCount)
{
	for (int i = 0; i < this->gridHeight * this->gridWidth; i++) {
		if (pos->x >= this->gridWidth) {
			pos->x = 0;
			pos->y++;
		}

		if (pos->y >= this->gridHeight)
			pos->y = 0;

		if (!this->grid[pos->x][pos->y].alive) 
			return true;

		pos->x++;
	}
	return false;
}

void Grid::PrintGrid() 
{
	string myOutput = "";
	for (int i = 0; i < this->gridWidth; i++)
	{
		myOutput += ".";
		for (int j = 0; j < this->gridHeight; j++)
		{
			if (this->grid[i][j].alive)
				myOutput += "o";
			else
				myOutput += " ";
			myOutput += ".";
		}
		myOutput += "\n";
	}
	cout << myOutput;
}

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
}

void Grid::GetNextCells()
{
	//std::vector<std::thread> threads;

	for (int x = 0; x < this->gridWidth; x++)
	{
		GetCellsInThreads(&x);
		//threads.emplace_back(&Grid::GetCellsInThreads, this, new int(x));  
	}

	/*for (int i = 0; i < threads.size(); ++i) {
		threads[i].join();
	}*/

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

template <size_t PatternSize, size_t GridSize>
std::bitset<PatternSize> Grid::ExtractPattern(int row, int col, int patternHeight, int patternWidth,
	int gridWidth, const std::bitset<GridSize>& gridSegment)
{
	std::bitset<PatternSize> extractedPattern;

	for (int j = 0; j < patternHeight; j++) 
	{
		for (int i = 0; i < patternWidth; i++) 
		{
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

template <size_t PatternSize>
std::bitset<PatternSize> Grid::GridGetBoxSelection(Vector2<int> const pos, int dimension, int historyIndex)
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
				selection[(i * dimension) + j] = (historyIndex == 0 ? 
					(this->grid[gridX][gridY].alive ? 1 : 0) : 
					(this->grid[gridX][gridY].GetHistory((historyIndex)) ? 1 : 0));
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

template <size_t PatternSize> bool Grid::GetHistory(Vector2<int> pos, PatternMask<PatternSize> pattern, int history)
{
	std::bitset<64> oldBits = GridGetBoxSelection<64>(pos, 8, history);
	if (CheckForOverlap<PatternSize>(pattern, oldBits))
		return true;
}

bool Grid::CheckForPattern(Pattern pattern) 
{
	for (int i = 0; i < this->gridWidth; i++)
	{
		for (int j = 0; j < this->gridHeight; j++)
		{
			if (this->grid[i][j].alive) 
			{
				if ((j > 0 && this->grid[i][j - 1].alive) || (i > 0 && this->grid[i - 1][j].alive)) 
					continue;  

				Vector2<int> pos = Vector2<int>(i - 2, j - 2);
				std::bitset<64> bits = GridGetBoxSelection<64>(pos, 8, 0);
				int aliveSquares = bits.count(); 

				if (aliveSquares < 3)
					continue;

				switch (pattern)
				{
					case Grid::Glider:

						if (aliveSquares < 5)
							continue;

						if (CheckForOverlap<25>(Patterns::gliderA1, bits) &&
							GetHistory<25>(pos, Patterns::gliderB1, 1) &&
							GetHistory<25>(pos, Patterns::gliderC1, 2) &&
							GetHistory<25>(pos, Patterns::gliderD1, 3))
							return true;
						else if (CheckForOverlap<25>(Patterns::gliderA2, bits) &&
							GetHistory<25>(pos, Patterns::gliderB2, 1) &&
							GetHistory<25>(pos, Patterns::gliderC2, 2) &&
							GetHistory<25>(pos, Patterns::gliderD2, 3))
							return true;
						else if (CheckForOverlap<25>(Patterns::gliderA3, bits) &&
							GetHistory<25>(pos, Patterns::gliderB3, 1) &&
							GetHistory<25>(pos, Patterns::gliderC3, 2) &&
							GetHistory<25>(pos, Patterns::gliderD3, 3))
							return true;

						else if (CheckForOverlap<25>(Patterns::gliderA4, bits) &&
							GetHistory<25>(pos, Patterns::gliderB4, 1) &&
							GetHistory<25>(pos, Patterns::gliderC4, 2) &&
							GetHistory<25>(pos, Patterns::gliderD4, 3))
							return true;

						break;
					case Grid::LWSS:

						if (aliveSquares < 9)
							continue;

						if (CheckForOverlap<42>(Patterns::lwssA1, bits) &&
							GetHistory<42>(pos, Patterns::lwssB1, 1) &&
							GetHistory<42>(pos, Patterns::lwssC1, 2) &&
							GetHistory<42>(pos, Patterns::lwssD1, 3))
							return true;
						else if (CheckForOverlap<42>(Patterns::lwssA2, bits) &&
							GetHistory<42>(pos, Patterns::lwssB2, 1) &&
							GetHistory<42>(pos, Patterns::lwssC2, 2) &&
						GetHistory<42>(pos, Patterns::lwssD2, 3))
							return true;
						else if (CheckForOverlap<42>(Patterns::lwssA3, bits) &&
							GetHistory<42>(pos, Patterns::lwssB3, 1) &&
							GetHistory<42>(pos, Patterns::lwssC3, 2) &&
							GetHistory<42>(pos, Patterns::lwssD3, 3))
							return true;

						else if (CheckForOverlap<42>(Patterns::lwssA4, bits) &&
							GetHistory<42>(pos, Patterns::lwssB4, 1) &&
							GetHistory<42>(pos, Patterns::lwssC4, 2) &&
							GetHistory<42>(pos, Patterns::lwssD4, 3))
							return true;

						break;
					case Grid::Toad:
						if (CheckForOverlap<24>(Patterns::toadA1, bits))
						{
							if (GetHistory<36>(pos, Patterns::toadB1, 1))
								if (GetHistory<24>(pos, Patterns::toadA1, 2))
									return true;
						}
						else if (CheckForOverlap<24>(Patterns::toadA2, bits)) {
							if (GetHistory<36>(pos, Patterns::toadB2, 1))
								if (GetHistory<24>(pos, Patterns::toadA2, 2))
									return true;
						}
						else if (CheckForOverlap<24>(Patterns::toadA3, bits)) {
							if (GetHistory<36>(pos, Patterns::toadB3, 1))
								if (GetHistory<24>(pos, Patterns::toadA3, 2))
									return true;
						}
						else if (CheckForOverlap<24>(Patterns::toadA4, bits)) {
							if (GetHistory<36>(pos, Patterns::toadB4, 1))
								if (GetHistory<24>(pos, Patterns::toadA4, 2))
									return true;
						}
							
						break;
					case Grid::Blinker:
						if (CheckForOverlap<15>(Patterns::blinkerA, bits))
						{
							if (GetHistory<15>(pos, Patterns::blinkerB, 1))
								return true;
						}
						else if (CheckForOverlap<15>(Patterns::blinkerB, bits)) 
						{
							if (GetHistory<15>(pos, Patterns::blinkerA, 1))
								return true;
						}
						break;
					case Grid::Beehive:
						if (CheckForOverlap<30>(Patterns::beehive, bits)) 
						{
							if (GetHistory<30>(pos, Patterns::beehive, 1))
								return true;
						}
						break;
					case Grid::Block:
						if (CheckForOverlap<16>(Patterns::block, bits)) {
							if (GetHistory<16>(pos, Patterns::block, 1))
								return true;
						}
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

	for (int i = 0; i < this->gridWidth; i++)
	{
		delete[] this->grid[i];
	}
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