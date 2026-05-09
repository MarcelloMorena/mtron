#pragma once

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include <memory>

class Mcp : public rclcpp::Node {
    public:
        Mcp();
    private:
        void initialise() const;
        void receive_user_coms(const std_msgs::msg::String &msg) const;

        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_user_coms_sub;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr m_mcp_coms_pub;
};