#include "McpSubsystem.hpp"

/**
 * TODO: write doc with @param and @return
 */
std::string McpSubsystem::initialMessage(int32_t const processSearchTaskId) const
{
    std::string msg =
        "Hello user, thank you for logging in.\n"
        "We need to test our process tracking on the Grid. I have deployed Tron to the Grid in sector " + std::to_string(processSearchTaskId) + ".\n"
        "Run the process tracker, enter his sector number and his new sector will quickly flash on the screen 2 digits at a time, with the entire sector number flashing at the end.\n"
        "Send back a message with his new sector to finalise the test, or message 'reset' to reset the test.\n";
    
    return msg;
}