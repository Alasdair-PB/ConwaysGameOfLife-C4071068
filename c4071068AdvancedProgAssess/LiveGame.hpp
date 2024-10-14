#include <iostream>
#include "Grid.hpp"
#include "Game.hpp"

#include <thread>
#include <chrono>
#include "Pattern.hpp"
#pragma once


using namespace std;

class LiveGame : Game
{

    private:
        int stepTime;
   

	public:

        LiveGame(int seed, int stepTime) : Game(seed), stepTime(stepTime) 
        {
            this->stepTime = stepTime;
        }

        LiveGame(int seed, int gridWidth, int gridHeight, int aliveCells, int maxSteps, int stepTime) : 
            Game(seed, gridWidth, gridHeight, aliveCells, maxSteps), stepTime(stepTime)
        {
            this->stepTime = stepTime;
        }


       void PlayGame() override
        {
           Grid grid = Grid(this->gridWidth, this->gridHeight, this->aliveCells, this->maxSteps, this->seed);
            grid.PrintGrid();

            while (true) {
                system("cls");
                UpdateResult gameState = grid.UpdateGrid();
                grid.PrintGrid();

                switch (gameState)
                {
                case StepCountExceeded:
                    grid.TerminateGrid();
                    return;
                    break;
                case Continue:
                    std::this_thread::sleep_for(std::chrono::milliseconds(this->stepTime));
                    break;
                case GridEmpty:
                    grid.TerminateGrid();
                    return;
                    break;
                default:
                    break;
                }
            }
        }


        bool FindPattern(Grid::Pattern pattern) override
        {
            Grid grid = Grid(this->gridWidth, this->gridHeight, this->aliveCells, this->maxSteps, this->seed);

            while (true) {
                UpdateResult gameState = grid.UpdateGrid(pattern);

                switch (gameState)
                {
                case StepCountExceeded:
                    grid.TerminateGrid();
                    return false;
                    break;
                case Continue:
                    break;
                case PatternFound:
                    cout << "Pattern Found in seed: " << seed << endl;
                    cout << "Found at step " << grid.stepCount << "." << endl;
                    grid.PrintGrid();
                    grid.TerminateGrid();
                    return true;
                    break;
                case GridEmpty:
                    grid.TerminateGrid();
                    return false;
                    break;
                default:
                    break;
                }
            }
        }

};