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
            WaitAndClear();
            int choice;
            string message = "Which pattern would you like to load?: ";
            choice = GetAllChoices(message);

            std::vector<SaveManager::GameSaveData> allData;

            switch (choice)
            {
                case Grid::Blinker:
                    allData = SaveManager::LoadAllData(fileName, Grid::Blinker);
                    break;
                case Grid::Block:
                    allData = SaveManager::LoadAllData(fileName, Grid::Block);
                    break;
                case Grid::Glider:
                    allData = SaveManager::LoadAllData(fileName, Grid::Glider);
                    break;
                case Grid::LWSS:
                    allData = SaveManager::LoadAllData(fileName, Grid::LWSS);
                    break;
                case Grid::Toad:
                    allData = SaveManager::LoadAllData(fileName, Grid::Toad);
                    break;
                case Grid::Beehive:
                    allData = SaveManager::LoadAllData(fileName, Grid::Beehive);
                    break;
                case Grid::Empty: 
                    break;
                default: 
                    break; 
            }


            int dataSize = allData.size();
            if (dataSize == 0)
                cout << "No data has been saved for this pattern. " << endl;
            else 
            {
                cout << "Save Data has been found!" << endl;
                choice = 0;

                if (dataSize > 1)
                {
                    WaitAndClear();
                    string message;
                    message += "Which save slot would you like to load?\n";
                    for (int i = 0; i < dataSize; i++) {
                        message += "Slot: " + std::to_string(i) + "\n";
                    }

                    choice = CinIntValueContinuous(message);

                    if (choice >= dataSize) 
                    {
                        WaitAndClear();
                        cout << "Input given does not correspond to options- defaulting to slot 0" << endl;
                        choice = 0;
                    }

                }

                WaitAndClear();
                ViewCreateSeed(allData[choice].seed, allData[choice].gridWidth, allData[choice].gridHeight, allData[choice].aliveCells);
            }

            WaitAndClear();
            GameModes();
        }

        void SaveSeed(Grid::Pattern pattern, int seed, int gridWidth, int gridHeight, int maxSteps, int aliveCells) {
            int choice;

            string message = "Would you like to save this seed?:\n(1) Yes\n(2) No\n";
            choice = CinIntValueContinuous(message);


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
            string message = "Would you like to see this seed develop?:\n(1) Yes\n(2) No\n";
            const int stepTime = 300;

            choice = CinIntValueContinuous(message);

            switch (choice)
            {
                case(1):
                {
                    WaitAndClear();
                    string message =  "Over how many steps would you like to see this step develop?";
                    choice = CinIntValueContinuous(message);
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
            string message = "Please input a " + name + " value:";
            choice = CinIntValueContinuous(message);
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

        int CinIntValueContinuous(string message)
        {
            while (true) {
                cout << message << endl;
                int choice;
                cin >> choice;

                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    WaitAndClear();
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    WaitAndClear();
                }
                else
                    return choice;
            }
        }

        int GetAllChoices(string message) 
        {
            while (true) {
                cout << message << endl;
                for (int firstItem = Grid::Glider; firstItem != Grid::Block + 1; firstItem++)
                {
                    cout << Grid::GetPatternName((Grid::Pattern)firstItem) << " (" << firstItem << ")" << endl;
                }
                int choice;
                cin >> choice;



                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    WaitAndClear();
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    WaitAndClear();
                }
                else
                    return choice;
            }
        }

        void NewSeed()
        {
            WaitAndClear();
            string message = "Is there a specific pattern you wish to search for?";

            int choice;
            choice = GetAllChoices(message);

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
            std::string message = "Would you like to:\n(1) Load Game By Seed\n(2) Start New Game\n(3) End Game\n";
            choice = CinIntValueContinuous(message);

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
                WaitAndClear();
                cout << "Your input did not correlate to any valid inputs." << endl;
                WaitAndClear();
                GameModes();
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
