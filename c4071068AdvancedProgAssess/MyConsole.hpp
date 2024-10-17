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
#include <thread>
#include <atomic>
#include <mutex>


using namespace std;

class MyConsole
{
    private:
        const std::string fileName = "myFile.json";
        atomic<bool> continueGame;
        atomic<int> threadsInUse;
        mutex seedMutex;
        mutex counterMutex;
        //int successfulSeed;

        struct Result {
            int successfulSeed; 
            int height;
            int width;
            int aliveCels;
        };

        Result result;

        MyConsole(const MyConsole&) = delete;
        MyConsole& operator=(const MyConsole&) = delete;

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

        void SetERNSearch(Grid::Pattern pattern)
        {
            int startSeed = GetValue("Start seed");
            int gridMaxWidth = GetValue("The maximum Grid Width");
            int gridMaxHeight = GetValue("The maximum Grid Height");
            int maxSteps = GetValue("The number of grids tested");
            int maxIterations = GetValue("The maximum iterations per grid before continuing (deciding impossible)");
            int aliveCells = GetValue("Maximum starting Alive Cell Count");

            FindBestCaseERN(pattern, startSeed, maxSteps, maxIterations, gridMaxWidth, gridMaxHeight);
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

        void OnExperimentsEnd(int seed, int startSeed) {
          cout << (seed - startSeed + 1) << " experiments have been run." << endl;
            cout << "Enter any key to proceed" << endl;
            char x;
            cin >> x;
            WaitAndClear();
        }

        void ContinueSearch(Grid::Pattern pattern, int seed, int gridWidth, int gridHeight, int maxSteps, int aliveCells)
        {
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
        }

        void Reset() {
            continueGame = true;
            threadsInUse = 0;

            result = { -1, 0, 0 ,0 };
           // successfulSeed = -1;
        }

        void increment() {
            lock_guard < std::mutex > guard(counterMutex);
            threadsInUse++;
        }

        void decrement() {
            lock_guard < std::mutex > guard(counterMutex);
            threadsInUse--;

        }

        void Search(Grid::Pattern pattern, int gridWidth, int gridHeight, int maxSteps, int aliveCells, Experimentation::SeedGenerator* generator)
        {
            increment();

            int seed = generator->GetNextSeed();
            Game myGame(seed, gridWidth, gridHeight, aliveCells, maxSteps);

            if (myGame.FindPattern(pattern)) {
                lock_guard<std::mutex> lock(seedMutex);
                if (continueGame) {
                    result = { seed, gridHeight, gridWidth ,aliveCells };
                    //successfulSeed = seed;
                    continueGame = false;
                }
            }
            else 
                decrement();
        }

        Result GetSuccessfulSeed() {
            lock_guard<std::mutex> lock(seedMutex);
            return result;
        }


        void CreateNewSeedSearch(Grid::Pattern pattern, int startSeed, int gridWidth, int gridHeight, int maxSteps, int aliveCells)
        {
            Reset();  
            WaitAndClear();
            Experimentation::SeedGenerator generator = Experimentation::SeedGenerator(startSeed);  

            cout << "Searching..." << endl;
            while (continueGame) 
            {
                if (threadsInUse < 3) {
                    std::thread searchThread(&MyConsole::Search, this, pattern, gridWidth, gridHeight, maxSteps, aliveCells, &generator);
                    searchThread.detach();
                    //CheckPause();
                }
            }

            int seed = GetSuccessfulSeed().successfulSeed;

            OnExperimentsEnd(seed, startSeed);
            ViewCreateSeed(seed, gridWidth, gridHeight, aliveCells);
            SaveSeed(pattern, seed, gridWidth, gridHeight, maxSteps, aliveCells);
            ContinueSearch(pattern, seed, gridWidth, gridHeight, maxSteps, aliveCells);

            std::cout << "What would you like to do now?" << std::endl;
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

        void FindBestCaseERN(Grid::Pattern pattern, int startSeed, int maxSteps, int maxIterations, int gridWidthMax, int gridHeightMax)
        {
            int bestSeed = 0;
            int bestERN = 999999;
            int bestGridWidth = 0;
            int bestGridHeight = 0;
            int bestAliveCells = 0;

            int gridWidth;
            int gridHeight;
            int aliveCells;

            srand(startSeed);

            for (int i = 0; i < maxSteps; i++) 
            {

                gridWidth = (rand() % gridWidthMax )+ 1;
                gridHeight = (rand() % gridHeightMax) + 1;
                aliveCells =( rand() % ((gridHeight * gridHeight)) - 1);

                if ((gridWidth + gridHeight + aliveCells) > bestERN)
                    Continue;

                WaitAndClear();
                Reset();

                Experimentation::SeedGenerator generator = Experimentation::SeedGenerator(startSeed);

                cout << "Searching..." << endl;
                int stepCap = 0; 
                while (continueGame)
                {

                    if (stepCap > maxIterations) {
                        break;
                    }
                    else if (threadsInUse < 2) {
                        stepCap++;
                        std::thread searchThread(&MyConsole::Search, this, pattern, gridWidth, gridHeight, maxSteps, aliveCells, &generator);
                        searchThread.detach();
                        //CheckPause();
                    }
    
                }

                if (stepCap > maxIterations)
                    continue;

                Result result = GetSuccessfulSeed();
                int seed = result.successfulSeed;
                int tempERN = result.aliveCels + result.width + result.height;

                if (tempERN < bestERN) {
                    bestERN = tempERN;
                    bestSeed = result.successfulSeed;
                    bestGridHeight = result.height;
                    bestGridWidth = result.width;
                    bestAliveCells = result.aliveCels;

                    cout << tempERN << endl;
                }
            }

            if (bestERN < 999999) {

                cout << "The best ERN value found for this seed is: " << bestERN << endl;
                SaveSeed(pattern, bestSeed, bestGridWidth, bestGridHeight, maxSteps, bestAliveCells);
            }
            else {
                cout << "No best case ERN could be found within this step count" << endl;
            }

            WaitAndClear();
            std::cout << "What would you like to do now?" << std::endl;
            GameModes();
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


        void NewERN()
        {
            WaitAndClear();
            string message = "Is there a specific pattern you wish to find an ERN for?";

            int choice;
            choice = GetAllChoices(message);

            switch (choice)
            {
            case Grid::Blinker:
                SetERNSearch(Grid::Blinker);
                break;
            case Grid::Block:
                SetERNSearch(Grid::Block);
                break;
            case Grid::Glider:
                SetERNSearch(Grid::Glider);
                break;
            case Grid::LWSS:
                SetERNSearch(Grid::LWSS);
                break;
            case Grid::Toad:
                SetERNSearch(Grid::Toad);
                break;
            case Grid::Beehive:
                SetERNSearch(Grid::Beehive);
                break;
            case Grid::Empty: break;
            default:  break;
            }
        }

        void GameModes() 
        {
            int choice;
            std::string message = "Would you like to:\n(1) Load Game By Seed\n(2) Start New Game\n(3) Find ERNs\n(4) End Game\n";
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
                NewERN();
                 break;
            case (4):
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

        MyConsole() : continueGame(true){}

        void Run() {
            Intro();
        }
};
