#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Subsystem to handle non-ROS related functionality of Grid class.
 * Saves MCP messages, tracks current sector location, manages the state for process tracking test.
 */
class GridSubsystem
{
    public:
        int32_t addProcess();
        void addMessage(std::string newMessage);
        std::string getMessage();
        bool processExists(int32_t processPos);
        int32_t pathTrace();
        int32_t finalisePath();
        bool checkSector(int32_t sectorGuess);
    private:
        int32_t m_processPos;
        std::string m_message;
        std::random_device m_rd;
        std::mt19937 m_generator {m_rd()};
        std::string m_path;
};