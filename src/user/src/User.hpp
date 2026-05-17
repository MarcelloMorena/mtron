#pragma once

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "std_msgs/msg/string.hpp"

#include "grid_interfaces/srv/get_messages.hpp"
#include "grid_interfaces/action/track_process.hpp"
#include "UserSubsystem.hpp"

/**
 * Handle user I/O for the Grid process tracking test.
 * 
 * - Client to grid/get_messages_request service.
 * - Client to grid/track_process_request action.
 * - Publisher to user/mcp_coms topic (used by MCP).
 */
class User : public rclcpp::Node {
    public:
        User();

    private:
        rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedPtr m_getMessagesClient;
        UserSubsystem m_userSubsystem;
        rclcpp_action::Client<grid_interfaces::action::TrackProcess>::SharedPtr m_gridActionClient;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr m_mcpComsPub;

        void initialise();
        void menu();
        void getMessages();
        void handleGetMessagesResponse(rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedFuture future);
        void sendMessage();
        void trackProcess();
        void goalResponseCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::SharedPtr const &goalHandle);
        void feedbackCallback(
            rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::SharedPtr const& goalHandle,
            std::shared_ptr<grid_interfaces::action::TrackProcess::Feedback const> const feedback
        );
        void resultCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::WrappedResult const &result);
};
