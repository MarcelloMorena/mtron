#include <iostream>
#include <random>

#include "GridSubsystem.hpp"

/**
 * Randomly generate 4 digit number for sector location, save to m_processPos and return.
 * 
 * @return 4 digit number between 1000 and 9999 representing sector location.
 */
int32_t GridSubsystem::addProcess()
{
    // Generate random number between 1000 and 9999.
    std::uniform_int_distribution<int32_t> distribution(1000, 9999);
    int32_t processPos = distribution(m_generator);
    m_processPos = processPos;
    return processPos;
}

/**
 * Save new MCP message to m_message.
 * 
 * @param newmessage New MCP message.
 */
void GridSubsystem::addMessage(std::string newMessage)
{
    m_message = newMessage;
}

/**
 * Return saves MCP message.
 * 
 * @return Saves MCP message.
 */
std::string GridSubsystem::getMessage()
{
    return m_message;
}

/**
 * Generate a random digit 1-9 and add to current path.
 * 
 * @return Most recent digit in the current path trace.
 * 
 * @note This doesn't generate 0 because when converting to string to int leading 0s will be removed.
 */
int32_t GridSubsystem::pathTrace()
{
    // Generate random digit between 1-9.
    std::uniform_int_distribution<int32_t> distribution(1, 9);
    int32_t nextPath = distribution(m_generator);

    // Append the new digit to the currently generating path.
    m_path = m_path + std::to_string(nextPath);

    // Return the new digit.
    return nextPath;
}

/**
 * Once all digits in new sector are generated, convert to int, save and return.
 * 
 * @return The new sector location made from the path.
 * 
 * @note Resets m_path to "" for next time pathTrace() is called.
 */
int32_t GridSubsystem::finalisePath()
{
    // Convert path to int, reset path, save sector location, and return.
    int32_t posAsInt = std::stoi(m_path);
    m_path = "";
    m_processPos = posAsInt;
    return posAsInt;
}

/**
 * Check if input process sector position is correct.
 * 
 * @param sectorPos Input sector position.
 * 
 * @return True if sector position is correct, False otherwise.
 * 
 */
bool GridSubsystem::checkSector(int32_t sectorPos)
{
    return sectorPos == m_processPos;
}
