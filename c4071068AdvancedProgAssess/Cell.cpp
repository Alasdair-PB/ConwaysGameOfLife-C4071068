#include "Cell.h"

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

void Cell::SetNextState() 
{
	this->alive = this->nextState;
}
