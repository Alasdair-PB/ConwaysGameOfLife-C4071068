#include <iostream>
#include "Grid.hpp"
#include <thread>
#include <chrono>
#pragma once

using namespace std;

class Game 
{

    protected:
        int seed;
        int gridWidth;
        int gridHeight;
        int aliveCells;
        int maxSteps;

    public:

        Game()
        {
            this->seed = 0;
            this->gridWidth = 32;
            this->gridHeight = 32;
            this->aliveCells = 99;
            this->maxSteps = 30;
        }

	    Game(int seed)
        {
            this->seed = seed;
            this->gridWidth = 32;
            this->gridHeight = 32;
            this->aliveCells = 99;
            this->maxSteps = 30;
        }

        Game(int seed, int gridWidth, int gridHeight, int aliveCells, int maxSteps)
        {
            this->seed = seed;
            this->gridWidth = gridWidth;
            this->gridHeight = gridHeight;
            this->aliveCells = aliveCells;
            this->maxSteps = maxSteps;
        }

	    virtual void PlayGame()
        {
           Grid grid = Grid(gridWidth, gridHeight, aliveCells, maxSteps, seed);

            while (true) {
                UpdateResult gameState = grid.UpdateGrid();
                switch (gameState)
                {
                case StepCountExceeded:
                    grid.TerminateGrid();
                    return;
                    break;
                case Continue:
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

    virtual bool FindPattern(Grid::Pattern pattern)
    {
        Grid grid = Grid(gridWidth, gridHeight, aliveCells, maxSteps, seed);
        int i = 0;

        while (true) {
            UpdateResult gameState = grid.UpdateGrid(pattern);
            i++;

            switch (gameState)
            {
            case StepCountExceeded:
                grid.TerminateGrid();
                return false;
                break;
            case Continue:
                break;
            case PatternFound:
                //cout << "Pattern Found in seed: " << seed << endl;
                //cout << "Found at step " << grid.stepCount << "." << endl;
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