#pragma once

#include <unordered_map>
#include <string>
#include <cstdint>
#include <vector>

/**
 * TODO: write doc
 */
class GridSubsystem
{
    public:
        int32_t addProcess(std::string processName);
        void addMessage(std::string newMessage);
        std::vector<std::string> getMessages();
    private:
        std::unordered_map<int32_t, std::string> m_processes;
        std::vector<std::string> m_messages;
};