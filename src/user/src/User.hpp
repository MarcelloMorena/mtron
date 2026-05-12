#pragma once
#include "UserSubsystem.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "grid_interfaces/srv/get_messages.hpp"

//#include <memory>

class User : public rclcpp::Node {
    public:
        User();

    private:
        void initialise();
        void menu();
        void getMessages();
        void handleGetMessagesResponse(rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedFuture future);

        UserSubsystem m_userSubsystem;
        rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedPtr m_getMessagesClient;

        // rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_mcp_coms_sub;
        // rclcpp::Publisher<std_msgs::msg::String>::SharedPtr m_user_coms_pub;

        // void reply() const;
        // void receive_mcp_coms(const std_msgs::msg::String &msg) const;
};