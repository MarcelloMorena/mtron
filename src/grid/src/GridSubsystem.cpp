#include <random>

#include "GridSubsystem.hpp"

/**
 * TODO: write doc with @param and @return
 */
int32_t GridSubsystem::addProcess(std::string processName)
{
    // Generate random number between 1000 and 9999
    std::uniform_int_distribution<int32_t> distribution(1000, 9999);
    int32_t processId = distribution(m_generator);

    // If ID is not unique (already in the map as a kay) keep generating new IDs
    while(m_processes.find(processId) != m_processes.end())
    {
        processId = distribution(m_generator);
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

bool GridSubsystem::processExists(int32_t processId)
{
    // Return true if processID exists, else false
    return(m_processes.find(processId) != m_processes.end());
}

/**
 * Generate 2 random digits, add to path, and return
 */
int32_t GridSubsystem::pathTrace()
{
    std::uniform_int_distribution<int32_t> distribution(10, 99);
    int32_t nextPath = distribution(m_generator);
    m_path = m_path + std::to_string(nextPath);
    return nextPath;
}

/**
 * Move process from to sector defined in m_path and clear m_path. Return final sector location.
 */
int32_t GridSubsystem::finalisePath(int32_t processId)
{
    std::string process = m_processes[processId];
    m_processes.clear();
    int32_t pathAsInt = std::stoi(m_path);
    m_processes[pathAsInt] = process;
    return pathAsInt;
}
