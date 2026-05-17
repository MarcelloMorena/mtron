#include "UserSubsystem.hpp"

#include <iostream>
#include <limits>
#include <string>

/**
 * Print welcome message.
 */
void UserSubsystem::welcome() {
    // Clear terminal, print welcome in ENCOM blue, reset colour after message.
    std::string welcome = std::string(m_kClearAnsi) + std::string(m_kBlueAnsi) +
                          "\nEstablishing Grid connection...\n"
                          "Connection established.\n"
                          "Welcome to the Grid.\n" + std::string(m_kResetAnsi);
    std::cout << welcome;
}

/**
 * Print ENCOM header and menu options, get and validate user input.
 * 
 * @return integer 1-4 depending on user input corresponding to menu choices, or -1
 *          if the input stream becomes invalid.
 * 
 * @note User input is validated and cannot be anything other than a single digit 1-4.
 */
int UserSubsystem::menu() {

    // Print ENCOM header and menu options, print user 'prompt' in blue then reset colour.
    std::string menuOptions = std::string(m_kMainframeHeader) +
                              "\n1 - Check messages\n"
                              "2 - Send message\n"
                              "3 - Track process\n"
                              "4 - Sign out\n" +
                              std::string(m_kBlueAnsi) + "User>> " + std::string(m_kResetAnsi);
    std::cout << menuOptions;

    // Get user input, loop until input is valid (single digit 1-4).
    std::string input;
    while(true){

        // Get input, if stream is invalid return -1.
        if(!std::getline(std::cin, input)){

            return -1;
        }
        std::cout << "\n";
        if (input == "1")
        {
            return 1;
        }
        else if (input == "2")
        {
            return 2;
        }
        else if (input == "3")
        {
            return 3;
        }
        else if (input == "4")
        {
            return 4;
        }
        else
        {
            std::cout << "Invalid menu option.\n";
        }
    }
}

/**
 * Clear terminal, print Grid message to screen in red.
 *
 * @param Message from the Grid as a string.
 */
void UserSubsystem::printMessage(std::string message)
{
    std::cout << m_kClearAnsi << m_kRedAnsi << message << m_kResetAnsi << "\n";
}

/**
 * Get and validate user's input for process sector.
 * 
 * @return User input as an integer.
 */
int UserSubsystem::getProcessPos()
{
    std::cout << "Enter process position: ";

    // Keep getting input until valid (i.e. is an int).
    int input;
    while(!(std::cin >> input))
    {
        std::cout << "Invalid input, please enter an integer: ";

        // Reset error state on input stream and flush input buffer of characters that weren't read.
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Remove trailing newline char and clear the terminal screen.
    std::cin.ignore();
    std::cout << m_kClearAnsi;
    return input;
}

/**
 * Get user input for message to MCP.
 * 
 * @return User input as a string.
 */
std::string UserSubsystem::messageUserInput()
{
    std::cout << "Enter message for MCP: ";
    std::string input;
    std::getline(std::cin, input);
    
    // Clear the terminal screen after input.
    std::cout << m_kClearAnsi << "\n";
    return input;
}
