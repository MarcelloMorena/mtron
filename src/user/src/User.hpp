#pragma once

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include <memory>

class User : public rclcpp::Node {
    public:
        User();
    private:
        void initialise() const;
        void receive_mcp_coms(const std_msgs::msg::String &msg) const;

        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_mcp_coms_sub;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr m_user_coms_pub;
};