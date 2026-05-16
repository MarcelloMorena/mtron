#pragma once

#include <memory>

#include "grid_interfaces/srv/add_process.hpp"
#include "grid_interfaces/srv/check_sector.hpp"
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
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_mcpComsSub;
        rclcpp::Client<grid_interfaces::srv::CheckSector>::SharedPtr m_checkSectorClient;

        void initialise();
        void sendInitialClientComs(rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedFuture future);
        void handleMcpComsSub(std_msgs::msg::String const& msg);
        void handleCheckSectorResponse(rclcpp::Client<grid_interfaces::srv::CheckSector>::SharedFuture future);
        void sendResetPositionToUser(rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedFuture future);
};
