#include "Grid.h"
#include <cstdlib>
#include <iostream> 
# include <windows.h>
#include <bitset>
#include "Vector2.h"
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


// Combine with default pattern check to avoid an insane amount of loops
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


// Make polymorhpic type for marks idk not neccessary though
namespace Patterns
{
	const std::bitset<16> block("0000011001100000");
}

bool HasOverlap(const std::bitset<64>& gridSegment, const std::bitset<16>& myPattern) 
{
	for (size_t i = 0; i <= 64 - 16; i++) 
	{
		std::bitset<16> to16((gridSegment >> i).to_ullong() & 0xFFFF); 

		if (myPattern == to16) 
			return true;
	}
	return false; 
}

// Hard coded rn to be 8 by 8
std::bitset<64> Grid::GridGetBoxSelection64(Vector2<int> const pos)
{
	std::bitset<64> selection;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int gridX = pos.x + i;
			int gridY = pos.y + j;

			if (gridX >= 0 && gridX < this->gridWidth && gridY >= 0 && gridY < this->gridHeight)
				selection[i * 8 + j] = this->grid[gridX][gridY].alive ? 1 : 0;
			else
				selection[i * 8 + j] = 0;
		}
	}
	return selection;
}

bool Grid::CheckForPattern(Pattern pattern) 
{
	for (int i = 0; i < this->gridWidth; i++)
	{
		for (int j = 0; j < this->gridHeight; j++)
		{
			if (this->grid[i][j].alive) 
			{
				Vector2<int> pos = Vector2<int>(i - 2, j - 2);
				std::bitset<64> bits = GridGetBoxSelection64(pos);
				cout << bits << endl;


				if (HasOverlap(bits, Patterns::block))
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