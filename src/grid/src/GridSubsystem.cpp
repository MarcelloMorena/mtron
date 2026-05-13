#include "GridSubsystem.hpp"

#include <random>

int32_t GridSubsystem::addProcess(std::string processName)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<int32_t> distribution(1000, 9999);
    int32_t processId = distribution(generator);

    // keep generating process ID if it is already in the map
    while(m_processes.find(processId) != m_processes.end())
    {
        processId = distribution(generator);
    }

    // add processName to map with processID key
    m_processes[processId] = processName;

    return processId;
}

void GridSubsystem::addMessage(std::string newMessage)
{
    m_messages.push_back(newMessage);
}

std::vector<std::string> GridSubsystem::getMessages()
{
    return m_messages;
}