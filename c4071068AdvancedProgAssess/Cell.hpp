#pragma once

//template <typename T>
class Cell
{
	public:

		bool alive;

		Cell() {
			alive = false;
			nextState = false;
		}

		void GetNextState(bool* neighbours, const int neighbourCount);
		void SetNextState();
	private:
		bool nextState;
		int IncrementCount(bool neighbourState);



};

