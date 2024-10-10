#include <iostream>
#include "Grid.hpp"
#include <thread>
#include <chrono>
#include "Pattern.hpp"
#include "Patterns.hpp"


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

bool SearchFor(Grid::Pattern pattern, int seed)
{
    Grid grid = Grid(32, 32, 99, 30, seed);

    while(true){
        UpdateResult gameState = grid.UpdateGrid(pattern);

        switch (gameState)
        {
            case StepCountExceeded:
                cout << "Exceeded Step count" << endl;
                grid.TerminateGrid();
                return false;
                break;
            case Continue:
                break;
            case PatternFound:
                cout << "Pattern Found" << endl;
                grid.TerminateGrid();
                return true;
                break;
            case GridEmpty:
                cout << "Empty grid" << endl;
                grid.TerminateGrid();
                return false;
                break;
            default:
                break;
        }
    }
}

void PrintGrid(int seed, const int period, const int stepTime) 
{
    Grid grid = Grid(32, 32, 99, 30, seed);
    grid.PrintGrid();

    while (true) {
        system("cls");
        UpdateResult gameState = grid.UpdateGrid();
        grid.PrintGrid();

        switch (gameState)
        {
            case StepCountExceeded:
                cout << "Exceeded Step count" << endl;
                grid.TerminateGrid();
                return;
                break;
            case Continue:
                std::this_thread::sleep_for(std::chrono::milliseconds(stepTime));
                break;
            case GridEmpty:
                cout << "Empty grid" << endl;
                grid.TerminateGrid();
                return;
                break;
            default:
                break;
            }
    }
}


int main()
{
    // max Step count not used!! 
    int seed = Experimentation::GetSeed();

    const int stepTime = 250; 
    //PrintGrid(seed, 100, 250);
    bool found = SearchFor(Grid::Pattern::Block , seed);

    std::cout << "Hello World!\n";
}
