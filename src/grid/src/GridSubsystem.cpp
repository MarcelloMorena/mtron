#include <iostream>
#include <random>

#include "GridSubsystem.hpp"

/**
 * TODO: write doc with @param and @return
 */
int32_t GridSubsystem::addProcess()
{
    // Generate random number between 1000 and 9999
    std::uniform_int_distribution<int32_t> distribution(1000, 9999);
    int32_t processPos = distribution(m_generator);
    m_processPos = processPos;
    return processPos;
}

/**
 * TODO: write doc with @param
 */
void GridSubsystem::addMessage(std::string newMessage)
{
    m_message = newMessage;
}

/**
 * TODO: write doc with @return
 */
std::string GridSubsystem::getMessage()
{
    return m_message;
}

bool GridSubsystem::processExists(int32_t processPos)
{
    // Return true if processPos is valid
    return(m_processPos == processPos);
}

/**
 * Generate random digits, add to path, and return
 */
int32_t GridSubsystem::pathTrace()
{
    std::uniform_int_distribution<int32_t> distribution(1, 9);
    int32_t nextPath = distribution(m_generator);
    m_path = m_path + std::to_string(nextPath);
    return nextPath;
}

/**
 * Move process to sector defined in m_path and clear m_path. Return final sector location.
 */
int32_t GridSubsystem::finalisePath()
{
    int32_t posAsInt = std::stoi(m_path);
    m_path = "";
    m_processPos = posAsInt;
    return posAsInt;
}

bool GridSubsystem::checkSector(int32_t sectorGuess)
{
    return sectorGuess == m_processPos;
}
