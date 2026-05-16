#pragma once

#include <string>

/**
 * TODO: write doc
 */
class McpSubsystem {
    public:
        // TODO: does this need to be const?
        std::string initialMessage(int32_t const processSearchTaskId) const;
        bool checkInputInteger(std::string const userMessageInput) const;
        bool checkResetInput(std::string const userMessageInput) const;
};