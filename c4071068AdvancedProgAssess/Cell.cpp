#include "Cell.hpp"
#include <bitset>

void Cell::GetNextState(bool* neighbours, const int neighbourCount)
{
	int aliveNeighbours = 0;

	for (int i = 0; i < neighbourCount; i++)
	{
		aliveNeighbours += IncrementCount(neighbours[i]);
	}

	if (alive && (aliveNeighbours == 2 || aliveNeighbours == 3))
		nextState = true;
	else if (aliveNeighbours == 3) 
		nextState = true;
	else 
		nextState = false;
}

int Cell::IncrementCount(bool neighbourState) {
	return neighbourState ? 1 : 0;
}


bool Cell::GetHistory(int shiftCount) 
{
	int index = this->historyIndex - shiftCount;

	if (index < 0)
		return false;

	return this->history[index];
}


void Cell::SetNextState() 
{
	if (this->historyIndex >= this->history.size()) 
	{
		this->history >>= 1;  
		this->historyIndex = this->history.size() - 1;
	}

	this->history[this->historyIndex] = this->alive;
	this->historyIndex++;

	this->alive = this->nextState;
}
