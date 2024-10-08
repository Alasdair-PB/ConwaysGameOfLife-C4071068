#include <iostream>
#include "Grid.h"
#include <thread>
#include <chrono>
using namespace std;

namespace Experimentation 
{
    int GetSeed() 
    {
        return 45233535 % 64234; // Has Block!!!!!!!!!!!!!!

        // 45233535 % 64234 block && blinker
        // 255543235235 % 64234  Boat && blinker
    }
}



int main()
{
    // max Step count not used!! 
    int seed = Experimentation::GetSeed();
    Grid grid = Grid(32, 32, 99, 30, seed);

    grid.PrintGrid();
    cout << endl << endl << endl;

    const int period = 100;
    const int stepTime = 250; 

    for (int i = 0; i < period; i++) 
    {
        bool continueGame = grid.UpdateGrid();
        system("cls"); 
        grid.PrintGrid();
        std::this_thread::sleep_for(std::chrono::milliseconds(stepTime));

        if (continueGame)
            break;
    }

    grid.TerminateGrid();
    std::cout << "Hello World!\n";
}
