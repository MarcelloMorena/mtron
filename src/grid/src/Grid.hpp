#pragma once

#include <string>
#include <vector>

#include "grid_interfaces/action/perform_task.hpp"
#include "grid_interfaces/srv/get_messages.hpp"
#include "grid_interfaces/srv/add_process.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "std_msgs/msg/string.hpp"

#include "GridSubsystem.hpp"

/**
 * TODO: Write class comment
 */
class Grid : public rclcpp::Node
{
public:
    Grid();

private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_gridComsSub;
    rclcpp::Service<grid_interfaces::srv::GetMessages>::SharedPtr m_getMessagesService;
    rclcpp::Service<grid_interfaces::srv::AddProcess>::SharedPtr m_addProcessService;
    rclcpp_action::Server<grid_interfaces::action::PerformTask>::SharedPtr m_performTaskActionServer;

    GridSubsystem m_gridSubsystem;

    void receiveGridComs(const std_msgs::msg::String::SharedPtr msg);

    void getMessagesService(
        std::shared_ptr<grid_interfaces::srv::GetMessages::Request> request,
        std::shared_ptr<grid_interfaces::srv::GetMessages::Response> response
    );

    void addProcessService(
        const std::shared_ptr<grid_interfaces::srv::AddProcess::Request> request,
        std::shared_ptr<grid_interfaces::srv::AddProcess::Response> response
    );

    rclcpp_action::GoalResponse handleGoal(
        rclcpp_action::GoalUUID const& uuid,
        std::shared_ptr<grid_interfaces::action::PerformTask::Goal const> goal
    );

    rclcpp_action::CancelResponse handleCancel(
        std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::PerformTask>> const goalHandle
    );

    void handleAccepted(
        std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::PerformTask>> const goalHandle
    );

    void performTask(
        std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::PerformTask>> const goalHandle
    );
};
