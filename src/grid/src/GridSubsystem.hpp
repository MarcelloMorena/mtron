#pragma once

#include <cstdint>
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
    private:
        std::unordered_map<int32_t, std::string> m_processes;
        std::vector<std::string> m_messages;
};