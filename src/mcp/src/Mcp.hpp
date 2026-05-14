#pragma once

#include <memory>

#include "grid_interfaces/srv/add_process.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "McpSubsystem.hpp"

/**
 * TODO: write doc
 */
class Mcp : public rclcpp::Node
{
    public:
        Mcp();
    private:
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr m_gridComsPub;
        rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedPtr m_addGridProcessClient;
        McpSubsystem m_mcpSubsystem;

        void initialise();
        void sendInitialClientComs(rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedFuture future);
};
