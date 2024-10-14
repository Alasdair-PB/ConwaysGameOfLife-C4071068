#include <iostream>
#include "Grid.hpp"
#include <thread>
#include <chrono>
#include "Pattern.hpp"
#include "Game.hpp"
#include "SeedGenerator.hpp"
#include "LiveGame.hpp"
#include <conio.h>

using namespace std;

class MyConsole
{
    private:

        void WaitAndClear() 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            system("cls");
        }

        void LoadSeed()
        {
            cout << "Not Implented- too bad!" << endl;
            WaitAndClear();
            GameModes();
        }

        void SaveSeed(int seed, int gridWidth, int gridHeight, int maxSteps, int aliveCells) {
            int choice;
            cout << "Would you like to save this seed:" << endl << "(1) Yes " << endl << "(2) No" << endl;
            cin >> choice;

            switch (choice)
            {
                case(1):
                    // save Seed
                    cout << "This Seed has been Saved" << endl;
                    WaitAndClear();

                    break;
                default:
                    break;
            }

        }

        // Maybe include current step count??
        void ViewCreateSeed(int seed, int gridWidth, int gridHeight, int maxSteps, int aliveCells)
        {
            int choice;
            cout << "Would you like to see this seed develop?:" << endl << "(1) Yes " << endl << "(2) No" << endl;
            const int stepTime = 300;
            cin >> choice;

            switch (choice)
            {
                case(1):
                {
                    LiveGame myGame = LiveGame(seed, gridWidth, gridHeight, aliveCells, maxSteps, stepTime);
                    myGame.PlayGame();
                    break;
                }
                default:
                    break;
            }

            cout << "Press any key to continue" << endl;
            char x;
            cin >> x;
            WaitAndClear();
        }

        int GetValue(string name) 
        {
            WaitAndClear();
            int choice;
            cout << "Please input a " << name << " value:" << endl;
            cin >> choice;
            return choice;
        }

        void SetSeedSearch(Grid::Pattern pattern)
        {
            int startSeed = GetValue("Start seed");
            int gridWidth = GetValue("Grid Width");
            int gridHeight = GetValue("Grid Height");
            int maxSteps = GetValue("Max Steps Count");
            int aliveCells = GetValue("Starting Alive Cell Count");

            CreateNewSeedSearch(pattern, startSeed, gridWidth, gridHeight, maxSteps, aliveCells);
        }

        void CheckPause() 
        {
            if (_kbhit()) {
                WaitAndClear();
                char x;
                cout << "Paused: enter a key to continue" << endl;
                cin >> x;
                WaitAndClear();
                cout << "Searching..." << endl;
            }
        }

        void CreateNewSeedSearch(Grid::Pattern pattern, int startSeed, int gridWidth, int gridHeight, int maxSteps, int aliveCells)
        {
            WaitAndClear();
            Experimentation::SeedGenerator generator = Experimentation::SeedGenerator(startSeed);
            bool continueGame = true;
            int seed;
            Game myGame;

            cout << "Searching..." << endl;
            while (continueGame)
            {
                seed = generator.GetNextSeed();
                myGame = Game(seed, gridWidth, gridHeight, aliveCells, maxSteps);
                continueGame = !(myGame.FindPattern(pattern));
                CheckPause();
            }

            cout << "Enter any key to proceed" << endl;
            char x;
            cin >> x;
            
            WaitAndClear();
            ViewCreateSeed(seed, gridWidth, gridHeight, maxSteps, aliveCells);
            SaveSeed(seed, gridWidth,gridHeight, maxSteps, aliveCells);

            WaitAndClear();
            int choice;
            cout << "Would you like to continue your search?:" << endl << "(1) Yes " << endl << "(2) No" << endl;
            cin >> choice;
            switch (choice)
            {
                case(1):
                {
                    CreateNewSeedSearch(pattern, ++seed, gridWidth, gridHeight, maxSteps, aliveCells);
                    break;
                }
                default:
                    break;
            }


            WaitAndClear();
            cout << "What would you like to do now?" << endl;
            GameModes();
        }

        void NewSeed()
        {
            WaitAndClear();
            int choice;
            std::cout << "Is there a specific pattern you wish to search for?" << endl;

            for (int firstItem = Grid::Glider; firstItem != Grid::Block + 1; firstItem++)
            {
                cout << Grid::GetPatternName((Grid::Pattern)firstItem) << " (" << firstItem << ")" << endl;
            }
            cin >> choice;

            switch (choice)
            {
            case Grid::Blinker:
                SetSeedSearch(Grid::Blinker);
                break;
            case Grid::Block:
                SetSeedSearch(Grid::Block);
                break;
            case Grid::Glider:
                SetSeedSearch(Grid::Glider);
                break;
            case Grid::LWSS:
                SetSeedSearch(Grid::LWSS);
                break;
            case Grid::Toad:
                SetSeedSearch(Grid::Toad);
                break;
            case Grid::Beehive:
                SetSeedSearch(Grid::Beehive);
                break;
            case Grid::Empty: break;
            default:  break;
            }
        }

        void GameModes() 
        {
            int choice;
            std::cout << "would you like to:" << endl << "(1) Load Game By Seed" << 
                endl << "(2) Start New Game" << endl << "(3) End Game" << endl;
            cin >> choice;

            switch (choice)
            {
            case (1):
                LoadSeed();
                break;

            case (2):
                NewSeed();
                break;
            case (3):
                return;
            default:
                cout << "Your input did not correlate to any valid inputs." << endl;
                break;
            }
        }

        void Intro()
        {
            std::cout << "Welcome to Conway's Game of life:";
            GameModes();
        }


    public:
        MyConsole() {};

        void Run() {
            Intro();
        }
};
