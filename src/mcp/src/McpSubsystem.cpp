#include <algorithm>
#include <sstream>

#include "McpSubsystem.hpp"

/**
 * Construct initial HELP message for the User, providing them with the first sector position.
 * 
 * @param sectorPos Tron's initial sector position to supply to the User.
 * 
 * @return HELP message with sectorPos embedded to send to the User on startup.
 */
std::string McpSubsystem::initialMessage(int32_t const sectorPos) const
{
    std::string msg =
        "Hello user, thank you for logging in.\n"
        "We need to test our process tracking on the Grid. I have deployed Tron to the Grid in sector " +std::to_string(sectorPos) + ".\n"
        "Run the process tracker, enter his sector number and his new sector will quickly flash on the screen a digit at a time.\n"
        "The entire sector number will flash once at the end.\n"
        "Send back a message with his new sector to finalise the test, or message 'reset' to reset the test.\nEND OF LINE";
    return msg;
}

/**
 * Check if user input is an integer.
 * 
 * @param userMessageInput Input from the user to check.
 * 
 * @return True if string is all digits, else False.
 */
bool McpSubsystem::checkInputInteger(std::string const userMessageInput) const
{
    // Convert user input to string stream.
    std::istringstream iss(userMessageInput);

    // Try to put the user input into an integer variable, if the stream is finished after the whole string is digits.
    int integer;
    bool isInt = (iss >> integer) && iss.eof();
    return isInt;
}

/**
 * Check if user input is 'reset'.
 * 
 * @param userMessageInput Input from the user to check.
 * 
 * @return True if string is 'reset', else False.
 * 
 * @note Case-insensitive.
 */
bool McpSubsystem::checkResetInput(std::string const userMessageInput) const
{
    std::string str = userMessageInput;
    // Convert all characters to lower case.
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    bool isReset = str == "reset";
    return isReset;
}
