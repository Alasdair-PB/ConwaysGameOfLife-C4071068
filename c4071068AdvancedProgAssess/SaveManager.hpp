#include <iostream>
#include <fstream>
#include <string>
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

        std::string toJSON(const std::string& slotName) const
        {
            return "{ \"slot\": \"" + slotName + "\", \"pattern\": " + std::to_string(static_cast<int>(pattern)) +
                ", \"gridWidth\": " + std::to_string(gridWidth) +
                ", \"gridHeight\": " + std::to_string(gridHeight) +
                ", \"aliveCells\": " + std::to_string(aliveCells) +
                ", \"seed\": " + std::to_string(seed) + " }";
        }

        static GameSaveData fromJSON(const std::string& jsonString)
        {
            GameSaveData data;
            sscanf_s(jsonString.c_str(), "{ \"slot\": \"%*s\", \"pattern\": %d, \"gridWidth\": %d, \"gridHeight\": %d, \"aliveCells\": %d, \"seed\": %d }",
                reinterpret_cast<int*>(&data.pattern),
                &data.gridWidth,
                &data.gridHeight,
                &data.aliveCells,
                &data.seed);
            return data;
        }
    };

    GameSaveData LoadData(const std::string& filename, Grid::Pattern pattern, int slotNumber)
    {
        std::ifstream inFile(filename);
        GameSaveData data = { Grid::Pattern::Empty, 0, 0, 0, 0 };

        if (inFile.is_open())
        {
            std::string line;
            std::string slotName = std::string(Grid::GetPatternName(pattern)) + std::to_string(slotNumber);
            while (std::getline(inFile, line))
            {
                if (line.find("\"slot\": \"" + slotName + "\"") != std::string::npos)
                {
                    data = GameSaveData::fromJSON(line);
                    break;
                }
            }
            inFile.close();
        }

        return data;
    }

    void SaveData(const std::string& filename, const GameSaveData& data)
    {
        std::ifstream inFile(filename);
        int slotNumber = 1;
        std::string baseSlotName = std::string(Grid::GetPatternName(data.pattern));


        if (inFile.is_open())
        {
            std::string line;
            while (std::getline(inFile, line))
            {
                if (line.find("\"slot\": \"" + baseSlotName) != std::string::npos)
                {
                    std::string existingSlot = line.substr(line.find("\"slot\": \"") + 9);
                    existingSlot = existingSlot.substr(0, existingSlot.find("\""));

                    int existingSlotNumber = 0;
                    sscanf_s(existingSlot.c_str() + baseSlotName.length(), "%d", &existingSlotNumber);
                    slotNumber = std::max(slotNumber, existingSlotNumber + 1);
                }
            }
            inFile.close();
        }

        std::ofstream outFile(filename, std::ios::app);
        if (outFile.is_open())
        {
            std::string slotName = baseSlotName + std::to_string(slotNumber);
            outFile << data.toJSON(slotName) << std::endl;
            outFile.close();
        }
    }
};
