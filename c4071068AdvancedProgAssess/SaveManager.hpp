//#include <nlohmann/json.hpp>
#include <iostream>
using namespace std;

class SaveManager
{
	struct GameSaveData 
	{
		int gridWidth;
		int gridHeight;
		int aliveCells;
		int seed;
	};


	public:
		SaveManager() {};
		
		GameSaveData LoadData() 
		{
			
		}

		GameSaveData SaveData() 
		{

		}

	private:

};