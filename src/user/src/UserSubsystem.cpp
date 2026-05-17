#include "UserSubsystem.hpp"

#include <iostream>
#include <limits>
#include <string>

/**
 * Print welcome message
 */
void UserSubsystem::welcome() {
    std::string welcome = std::string(m_ClearAnsi) + std::string(m_MainframeHeader) + std::string(m_BlueAnsi) +
                          "\nEstablishing Grid connection...\n"
                          "Connection established.\n"
                          "Welcome to the Grid.\n" + std::string(m_ResetAnsi);

    std::cout << welcome;
}

/**
 * Print menu options and get user input
 * 
 * TODO: write @return
 */
int UserSubsystem::menu() {
    std::string menuOptions = "\n1 - Check messages\n"
                              "2 - Send message\n"
                              "3 - Track process\n" +
                              std::string(m_BlueAnsi) + "User>> ";
    std::cout << menuOptions;
    
    std::string input;

    // Get user input, loop until input is valid
    while(true){

        std::cout << m_ResetAnsi;

        // Get input, if stream is invalid return
        if(!std::getline(std::cin, input)){
            std::cout << m_ResetAnsi;
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
        else
        {
            std::cout << "Invalid menu option.\n";
        }
    }
}

/**
 * Print Grid messages to screen
 * 
 * TODO: write @param
 */
void UserSubsystem::printMessage(std::string message)
{
    std::cout << m_ClearAnsi << m_RedAnsi << message << m_ResetAnsi << "\n";
}

int UserSubsystem::getProcessPos()
{
    std::cout << "Enter process position: ";
    int input;
    
    // Keep getting input until valid (i.e. is an int)
    while(!(std::cin >> input))
    {
        std::cout << "Invalid input, please enter an integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    // Remove trailing newline char
    std::cin.ignore();
    std::cout << m_ClearAnsi;
    return input;
}

std::string UserSubsystem::messageUserInput()
{
    std::string input;
    std::cout << "Enter message for MCP: ";
    std::getline(std::cin, input);
    std::cout << m_ClearAnsi << "\n";
    return input;
}
