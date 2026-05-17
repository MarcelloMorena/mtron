#pragma once

#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "std_msgs/msg/string.hpp"

#include "grid_interfaces/action/track_process.hpp"
#include "grid_interfaces/srv/add_process.hpp"
#include "grid_interfaces/srv/check_sector.hpp"
#include "grid_interfaces/srv/get_messages.hpp"
#include "GridSubsystem.hpp"

/**
 * The Grid.
 * Keeps messages from MCP for the User to retrieve.
 * Runs process tracker test.
 *
 * - Subscriber to mcp/grid_coms.
 * - Server to grid/get_messages_request service.
 * - Server to grid/add_process_request service.
 * - Server to grid/track_process_request action.
 * - Server to grid/check_sector_request service.
 */
class Grid : public rclcpp::Node
{
public:
    Grid();

private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_gridComsSub;
    rclcpp::Service<grid_interfaces::srv::GetMessages>::SharedPtr m_getMessagesService;
    rclcpp::Service<grid_interfaces::srv::AddProcess>::SharedPtr m_addProcessService;
    rclcpp_action::Server<grid_interfaces::action::TrackProcess>::SharedPtr m_trackProcessActionServer;
    GridSubsystem m_gridSubsystem;
    rclcpp::Service<grid_interfaces::srv::CheckSector>::SharedPtr m_checkSectorService;

    void receiveGridComs(std_msgs::msg::String::SharedPtr const msg);
    void getMessagesService(
        std::shared_ptr<grid_interfaces::srv::GetMessages::Request> const request,
        std::shared_ptr<grid_interfaces::srv::GetMessages::Response> response
    );
    void addProcessService(
        std::shared_ptr<grid_interfaces::srv::AddProcess::Request> const request,
        std::shared_ptr<grid_interfaces::srv::AddProcess::Response> response
    );
    rclcpp_action::GoalResponse handleGoal(
        rclcpp_action::GoalUUID const &uuid,
        std::shared_ptr<grid_interfaces::action::TrackProcess::Goal const> goal
    );
    rclcpp_action::CancelResponse handleCancel(
        std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle
    );
    void handleAccepted(
        std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle
    );
    void trackProcess(
        std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle
    );
    void checkSectorService(
        const std::shared_ptr<grid_interfaces::srv::CheckSector::Request> request,
        std::shared_ptr<grid_interfaces::srv::CheckSector::Response> response
    );
};
