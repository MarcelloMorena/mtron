#pragma once

#include <string>
#include <string_view>
#include <vector>

/**
 * Subsystem to handle non-ROS related functionality of User class, mainly I/O.
 */
class UserSubsystem {
    public:
        void welcome();
        int menu();
        void printMessage(std::string message);
        int getProcessPos();
        std::string messageUserInput();
    private:
        static constexpr std::string_view m_kResetAnsi = "\033[0m";
        static constexpr std::string_view m_kBlueAnsi = "\033[36m";
        static constexpr std::string_view m_kRedAnsi = "\033[31m";
        static constexpr std::string_view m_kClearAnsi = "\033[2J\033[1;1H";
        static constexpr std::string_view m_kMainframeHeader = "\033[36m===============\nENCOM MAINFRAME\n===============\033[0m\n";
};