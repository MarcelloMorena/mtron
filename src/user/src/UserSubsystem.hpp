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
        void printMessages(std::vector<std::string> messages);
};