#include "Grid.h"
#include <cstdlib>
#include <iostream> 
# include <windows.h>
#include <bitset>
#include "Vector2.h"

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
		int x = rand() % this->gridWidth; 
		int y = rand() % this->gridHeight; 

		Vector2<int> pos = Vector2<int>(x,y);
		pos = pos + 10; // Check he is happy with this kinda of operator overloading

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

void Grid::GetNextCells() {
	bool* neighbours = new bool[8];
	for (int x = 0; x < this->gridWidth; x++)
	{
		for (int y = 0; y < this->gridHeight; y++)
		{
			int index = 0;

			for (int offsetX = -1; offsetX <= 1; offsetX++)
			{
				for (int offsetY = -1; offsetY <= 1; offsetY++)
				{
					if (offsetX == 0 && offsetY == 0)
						continue;

					int neighbourX = x + offsetX;
					int neighbourY = y + offsetY;

					if (neighbourX >= 0 && neighbourX < gridWidth && neighbourY >= 0 && neighbourY < gridHeight)
						neighbours[index++] = this->grid[neighbourX][neighbourY].alive;
					else
						neighbours[index++] = false;
				}
			}

			this->grid[x][y].GetNextState(neighbours, 8);
		}
	}
	delete[] neighbours;
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


// Make polymorhpic type for marks idk not neccessary though
namespace Patterns
{
	const std::bitset<16> block("0000011001100000");
}

//	Could make this a custom opperator of in if that exists?
bool HasOverlap(const std::bitset<64>& gridSegment, const std::bitset<16>& myPattern)
{
	for (size_t i = 0; i <= 64 - 16; i++) 
	{
		std::bitset<16> to16 = ((gridSegment).any() >> i);
		if ((myPattern & to16) == 0) {
			return true;
		}
	}
	return false;
}

std::bitset<64> Grid::GridGetBoxSelection16(Vector2<int> pos, int boundryBoxWidth, int boundryBoxHeight)
{
	std::bitset<64> selection;
	for (int i = 0; i < boundryBoxWidth; i++) {
		for (int j = 0; j < boundryBoxHeight; j++){
			if ((pos.x + i) < gridWidth && (pos.y + j) < gridHeight)
				selection[i + j] = this->grid[pos.x + i, pos.y + j]->alive;
			else
				selection[i + j] = 0;
		} 
	}
	return selection;
}


bool Grid::CheckForPattern(Pattern pattern) 
{
	std::bitset<64> i = GridGetBoxSelection16(Vector2<int>(0,1), 8, 8);
	if (HasOverlap(i, Pattern::Block)) {
		cout << endl << "Block found!!" << endl;
		return true;
	}

	//cout << endl << HasOverlap(i, Pattern::Block) << ": overlap" << endl;

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

bool Grid::UpdateGrid() 
{
	GetNextCells();
	SetNextCells();
	return CheckForPattern(Empty);
}

bool Grid::UpdateGrid(Pattern endsOn)
{
	GetNextCells();
	SetNextCells();

	if (CheckForPattern(Empty))
		return false;
	else 
		return CheckForPattern(endsOn);
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