#pragma once

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "grid_interfaces/srv/get_messages.hpp"

#include <string>
#include <vector>

/**
 * TODO: Write class comment
 */
class Grid : public rclcpp::Node
{
public:
    Grid();

private:
    void receiveGridComs(const std_msgs::msg::String::SharedPtr msg);
    void getMessagesService(
        const std::shared_ptr<grid_interfaces::srv::GetMessages::Request> request,
        std::shared_ptr<grid_interfaces::srv::GetMessages::Response> response
    );

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_gridComsSub;
    rclcpp::Service<grid_interfaces::srv::GetMessages>::SharedPtr m_getMessagesService;

    std::vector<std::string> m_msgs;
};