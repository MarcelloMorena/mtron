#pragma once

#include "grid_interfaces/srv/get_messages.hpp"
#include "rclcpp/rclcpp.hpp"
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

        void initialise();
        void menu();
        void getMessages();
        void handleGetMessagesResponse(rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedFuture future);
};
