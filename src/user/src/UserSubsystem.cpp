#include "UserSubsystem.hpp"

#include <iostream>
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
                              "3 - Perform task\n";
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