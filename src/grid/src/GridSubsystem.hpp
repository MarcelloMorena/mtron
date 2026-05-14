#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * TODO: write doc
 */
class GridSubsystem
{
    public:
        int32_t addProcess(std::string processName);
        void addMessage(std::string newMessage);
        // TODO: does making the vector returned const mean calling methods can't add/remove from it?
        std::vector<std::string> getMessages();
        bool processExists(int32_t processId);
        int32_t pathTrace();
        int32_t finalisePath(int32_t processId);
    private:
        std::unordered_map<int32_t, std::string> m_processes;
        std::vector<std::string> m_messages;
        std::random_device m_rd;
        std::mt19937 m_generator {m_rd()};
        std::string m_path;
};