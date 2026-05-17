#pragma once

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "grid_interfaces/srv/add_process.hpp"
#include "grid_interfaces/srv/check_sector.hpp"
#include "McpSubsystem.hpp"

/**
 * Master Control Program.
 * Handles communication with the User to manage the Grid process tracking test.
 *
 * - Publisher to mcp/grid_coms topic (used by Grid).
 * - Client to grid/add_process_request service.
 * - Subscriber to user/mcp_coms topic.
 * - Client to grid/check_sector_request service.
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
