#include <iostream>
#include "Grid.hpp"
#include <thread>
#include <chrono>
#include "Pattern.hpp"
#include "Game.hpp"
#include "SeedGenerator.hpp"
#include "SaveManager.hpp"
#include "LiveGame.hpp"
#include <conio.h>

using namespace std;

class MyConsole
{
    private:

        const string fileName = "myFile.json";

        void WaitAndClear() 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            system("cls");
        }

        void LoadSeed()
        {
            int choice;
            cout << "Which pattern would you like to load?: " << endl;
            choice = GetAllChoices();

            SaveManager::GameSaveData saveData = { Grid::Pattern::Empty, 0, 0, 0, 0 }; 

            switch (choice)
            {
                case Grid::Blinker:
                    saveData = SaveManager::LoadData(fileName, Grid::Blinker, 1);
                    break;
                case Grid::Block:
                    saveData = SaveManager::LoadData(fileName, Grid::Block, 1);
                    break;
                case Grid::Glider:
                    saveData = SaveManager::LoadData(fileName, Grid::Glider, 1);
                    break;
                case Grid::LWSS:
                    saveData = SaveManager::LoadData(fileName, Grid::LWSS, 1);
                    break;
                case Grid::Toad:
                    saveData = SaveManager::LoadData(fileName, Grid::Toad, 1);
                    break;
                case Grid::Beehive:
                    saveData = SaveManager::LoadData(fileName, Grid::Beehive, 1);
                    break;
                case Grid::Empty: 
                    break;
                default: 
                    break; 
            }

            if (saveData.pattern == Grid::Empty)
                cout << "No data has been saved for this pattern. " << endl;
            else 
            {
                ViewCreateSeed(saveData.seed, saveData.gridWidth, saveData.gridHeight, saveData.aliveCells);
            }

            WaitAndClear();
            GameModes();
        }

        void SaveSeed(Grid::Pattern pattern, int seed, int gridWidth, int gridHeight, int maxSteps, int aliveCells) {
            int choice;
            cout << "Would you like to save this seed:" << endl << "(1) Yes " << endl << "(2) No" << endl;
            cin >> choice;

            switch (choice)
            {
                case(1): 
                {
                    SaveManager::GameSaveData saveData = { pattern, gridWidth, gridHeight, aliveCells, seed};
                    SaveManager::SaveData(fileName, saveData);
                    cout << "This Seed has been Saved" << endl;
                    WaitAndClear();

                    break;
                }
                default:
                    break;
            }

        }

        
        void ViewCreateSeed(int seed, int gridWidth, int gridHeight, int aliveCells)
        {
            int choice;
            cout << "Would you like to see this seed develop?:" << endl << "(1) Yes " << endl << "(2) No" << endl;
            const int stepTime = 300;
            cin >> choice;

            switch (choice)
            {
                case(1):
                {
                    WaitAndClear();
                    cout << "Over how many steps would you like to see this step develop?" << endl;
                    cin >> choice;

                    LiveGame myGame = LiveGame(seed, gridWidth, gridHeight, aliveCells, choice, stepTime);
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
            ViewCreateSeed(seed, gridWidth, gridHeight, aliveCells);
            SaveSeed(pattern, seed, gridWidth,gridHeight, maxSteps, aliveCells);

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

        int GetAllChoices() 
        {
            for (int firstItem = Grid::Glider; firstItem != Grid::Block + 1; firstItem++)
            {
                cout << Grid::GetPatternName((Grid::Pattern)firstItem) << " (" << firstItem << ")" << endl;
            }
            int choice;
            cin >> choice;
            return choice;
        }

        void NewSeed()
        {
            WaitAndClear();
            std::cout << "Is there a specific pattern you wish to search for?" << endl;

            int choice;
            choice = GetAllChoices();

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
