#include <algorithm>
#include <sstream>

#include "McpSubsystem.hpp"

/**
 * TODO: write doc with @param and @return
 */
std::string McpSubsystem::initialMessage(int32_t const processSearchTaskId) const
{
    std::string msg =
        "Hello user, thank you for logging in.\n"
        "We need to test our process tracking on the Grid. I have deployed Tron to the Grid in sector " + std::to_string(processSearchTaskId) + ".\n"
        "Run the process tracker, enter his sector number and his new sector will quickly flash on the screen a digit at a time.\n"
        "The entire sector number will flash once at the end.\n"
        "Send back a message with his new sector to finalise the test, or message 'reset' to reset the test.\nEND OF LINE";
    
    return msg;
}

bool McpSubsystem::checkInputInteger(std::string const userMessageInput) const
{
    std::istringstream iss(userMessageInput);
    int integer;

    // Try to put the user input into an integer variable, if the stream is finished the whole string is an integer
    bool isInt = (iss >> integer) && iss.eof();
    return isInt;
}

bool McpSubsystem::checkResetInput(std::string const userMessageInput) const
{
    std::string str = userMessageInput;
    // Convert all characters to lower case
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    bool isReset = str == "reset";
    return isReset;
}
