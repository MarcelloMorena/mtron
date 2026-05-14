#include <random>

#include "GridSubsystem.hpp"

/**
 * TODO: write doc with @param and @return
 */
int32_t GridSubsystem::addProcess(std::string processName)
{
    // Generate random number between 1000 and 9999
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<int32_t> distribution(1000, 9999);
    int32_t processId = distribution(generator);

    // If ID is not unique (already in the map as a kay) keep generating new IDs
    while(m_processes.find(processId) != m_processes.end())
    {
        processId = distribution(generator);
    }

    // Add processName to map with processID key
    m_processes[processId] = processName;
    return processId;
}

/**
 * TODO: write doc with @param
 */
void GridSubsystem::addMessage(std::string newMessage)
{
    m_messages.push_back(newMessage);
}

/**
 * TODO: write doc with @return
 */
std::vector<std::string> GridSubsystem::getMessages()
{
    return m_messages;
}