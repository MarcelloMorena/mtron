#pragma once

#include <string>
#include <vector>

/**
 * TODO: Write class comment
 */
class UserSubsystem {
    public:
        void welcome();
        int menu();
        void printMessage(std::string message);
        int getProcessPos();
        std::string messageUserInput();
};