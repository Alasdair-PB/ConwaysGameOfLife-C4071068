#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Grid.hpp"

namespace SaveManager
{
    struct GameSaveData
    {
        Grid::Pattern pattern;
        int gridWidth;
        int gridHeight;
        int aliveCells;
        int seed;

        string toJSON(const string& slotName) const
        {
            return "{ \"slot\": \"" + slotName + "\", \"pattern\": " + to_string(static_cast<int>(pattern)) +
                ", \"gridWidth\": " + to_string(gridWidth) +
                ", \"gridHeight\": " + to_string(gridHeight) +
                ", \"aliveCells\": " + to_string(aliveCells) +
                ", \"seed\": " + to_string(seed) + " }";
        }

        static GameSaveData fromJSON(const string& jsonString)
        {
            GameSaveData data;
            int patternValue = 0;
            char slot[32];

            sscanf_s(jsonString.c_str(), "{ \"slot\": \"%31[^\"]\", \"pattern\": %d, \"gridWidth\": %d, \"gridHeight\": %d, \"aliveCells\": %d, \"seed\": %d }",
                slot, (unsigned)_countof(slot),
                &patternValue,
                &data.gridWidth,
                &data.gridHeight,
                &data.aliveCells,
                &data.seed);

            data.pattern = static_cast<Grid::Pattern>(patternValue);
            return data;
        }
    };


    GameSaveData LoadData(const string& filename, Grid::Pattern pattern, int slotNumber)
    {
        ifstream inFile(filename);
        GameSaveData data = { Grid::Pattern::Empty, 0, 0, 0, 0 };

        if (inFile.is_open())
        {
            string line;
            string slotName = string(Grid::GetPatternName(pattern)) + to_string(slotNumber);
            while (getline(inFile, line))
            {
                if (line.find("\"slot\": \"" + slotName + "\"") != string::npos)
                {
                    data = GameSaveData::fromJSON(line);
                    break;
                }
            }
            inFile.close();
        }

        return data;
    }


    vector<GameSaveData> LoadAllData(const string& filename, Grid::Pattern pattern)
    {
        ifstream inFile(filename);
        vector<GameSaveData> matchingData;

        if (inFile.is_open())
        {
            string line;
            string patternName = string(Grid::GetPatternName(pattern));

            while (getline(inFile, line))
            {
                if (line.find("\"slot\": \"" + patternName) != string::npos)
                {
                    GameSaveData data = GameSaveData::fromJSON(line);
                    matchingData.push_back(data);
                }
            }
            inFile.close();
        }
        return matchingData;
    }



    void SaveData(const string& filename, const GameSaveData& data)
    {
        ifstream inFile(filename);
        int slotNumber = 1;
        string baseSlotName = string(Grid::GetPatternName(data.pattern));


        if (inFile.is_open())
        {
            string line;
            while (getline(inFile, line))
            {
                if (line.find("\"slot\": \"" + baseSlotName) != string::npos)
                {
                    string existingSlot = line.substr(line.find("\"slot\": \"") + 9);
                    existingSlot = existingSlot.substr(0, existingSlot.find("\""));

                    int existingSlotNumber = 0;
                    sscanf_s(existingSlot.c_str() + baseSlotName.length(), "%d", &existingSlotNumber);
                    slotNumber = max(slotNumber, existingSlotNumber + 1);
                }
            }
            inFile.close();
        }

        ofstream outFile(filename, ios::app);
        if (outFile.is_open())
        {
            string slotName = baseSlotName + to_string(slotNumber);
            outFile << data.toJSON(slotName) << endl;
            outFile.close();
        }
    }
};
