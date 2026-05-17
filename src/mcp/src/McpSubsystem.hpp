#pragma once

#include <string>

/**
 * Subsystem to handle non-ROS related functionality of Mcp class, mainly I/O.
 */
class McpSubsystem {
    public:
        std::string initialMessage(int32_t const sectorPos) const;
        bool checkInputInteger(std::string const userMessageInput) const;
        bool checkResetInput(std::string const userMessageInput) const;
};
