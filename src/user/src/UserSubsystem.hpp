#pragma once

#include <string>
#include <string_view>
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
    private:
        static constexpr std::string_view m_ResetAnsi = "\033[0m";
        static constexpr std::string_view m_BlueAnsi = "\033[36m";
        static constexpr std::string_view m_RedAnsi = "\033[31m";
        static constexpr std::string_view m_ClearAnsi = "\033[2J\033[1;1H";
        static constexpr std::string_view m_MainframeHeader = "\033[36m===============\nENCOM MAINFRAME\n===============\033[0m\n";
};