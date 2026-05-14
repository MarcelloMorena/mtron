#pragma once

#include "grid_interfaces/srv/get_messages.hpp"
#include "grid_interfaces/action/perform_task.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "std_msgs/msg/string.hpp"

#include "UserSubsystem.hpp"

/**
 * TODO: write doc
 */
class User : public rclcpp::Node {
    public:
        User();

    private:
        rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedPtr m_getMessagesClient;
        UserSubsystem m_userSubsystem;
        rclcpp_action::Client<grid_interfaces::action::PerformTask>::SharedPtr m_gridActionClient;

        void initialise();
        void menu();
        void getMessages();
        void handleGetMessagesResponse(rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedFuture future);
        void trackProcess();
        void goalResponseCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::PerformTask>::SharedPtr const& goalHandle);
        void feedbackCallback(
            rclcpp_action::ClientGoalHandle<grid_interfaces::action::PerformTask>::SharedPtr,
            std::shared_ptr<grid_interfaces::action::PerformTask::Feedback const> const feedback
        );
        void resultCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::PerformTask>::WrappedResult const& result);
};
