#include "Grid.hpp"

/**
 * Create a node with label "grid", set m_gridComsSub as a subscription to grid_coms topic,
 * set m_getMessagesService as a service called get_messages linked to the getMessagesService method
 */
Grid::Grid() : rclcpp::Node("grid")
{
    m_gridComsSub = this->create_subscription<std_msgs::msg::String>(
        "grid_coms",
        rclcpp::QoS(10).transient_local().reliable(),
        std::bind(&Grid::receiveGridComs, this, std::placeholders::_1)
    );

    m_getMessagesService = this->create_service<grid_interfaces::srv::GetMessages>(
        "get_messages",
        std::bind(&Grid::getMessagesService, this, std::placeholders::_1, std::placeholders::_2)
    );
}

/**
 * Callback for messages posted to grid_coms topic
 */
void Grid::receiveGridComs(const std_msgs::msg::String::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), "Received coms from MCP.");
    m_msgs.push_back(msg->data);
}

/**
 * Callback for requests made to the get_messages service.
 */
void Grid::getMessagesService(const std::shared_ptr<grid_interfaces::srv::GetMessages::Request> request,
                                std::shared_ptr<grid_interfaces::srv::GetMessages::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "User requested messages");
    response->messages = m_msgs;
}
