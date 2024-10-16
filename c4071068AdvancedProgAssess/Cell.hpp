#pragma once
#include <bitset>
//template <typename T>
class Cell
{
	public:

		bool alive;

		Cell() {
			alive = false;
			nextState = false;
			history = std::bitset<12>();
			historyIndex = 0;
		}

		void GetNextState(bool* neighbours, const int neighbourCount);
		void SetNextState();
		bool GetHistory(int shiftCount);
	private:
		int historyIndex;
		std::bitset<12> history;
		bool nextState;
		int IncrementCount(bool neighbourState);



};

