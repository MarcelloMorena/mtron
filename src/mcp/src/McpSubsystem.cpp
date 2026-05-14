#include "McpSubsystem.hpp"

/**
 * TODO: write doc with @param and @return
 */
std::string McpSubsystem::initialMessage(int32_t const processSearchTaskId) const
{
    std::string msg =
        "Hello user, thank you for logging in.\n"
        "A rogue program has invaded the Grid is corrupting sectors!\n"
        "We need to deploy Tron to save us, but his process ID has been erased from my memory files.\n"
        "Use admin task " + std::to_string(processSearchTaskId) + " to find out the process ID for Tron, message me back with his ID and I will spin him up.\n";
    
    return msg;
}