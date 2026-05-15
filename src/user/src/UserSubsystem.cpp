#include "UserSubsystem.hpp"

#include <iostream>
#include <limits>
#include <string>

/**
 * Print welcome message
 */
void UserSubsystem::welcome() {
    std::string welcome = "===============\nENCOM MAINFRAME\n===============\n"
                          "\nEstablishing Grid connection...\n"
                          "Connection established.\n"
                          "Welcome to the Grid.\n\n";

    std::cout << welcome;
}

/**
 * Print menu options and get user input
 * 
 * TODO: write @return
 */
int UserSubsystem::menu() {
    std::string menuOptions = "1 - Check messages\n"
                              "2 - Send message\n"
                              "3 - Track process\n";
    std::cout << menuOptions;
    
    std::string input;

    // Get user input, loop until input is valid
    while(true){

        // If input stream is invalid
        if(!std::getline(std::cin, input)){
            return -1;
        }

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
void UserSubsystem::printMessages(std::vector<std::string> messages)
{
    for (auto const& msg : messages)
    {
        std::cout << msg << "\n";
    }
}

int UserSubsystem::getProcessId()
{
    std::cout << "Enter process ID.\n";
    int input;
    
    // Keep getting input until valid (i.e. is an int)
    while(!(std::cin >> input))
    {
        std::cout << "Invalid input, please enter an integer.";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return input;
}
