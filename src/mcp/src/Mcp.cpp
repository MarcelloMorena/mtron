#include "Mcp.hpp"
//#include <chrono>

/**
 * Create a node with label "mcp" and set m_gridComsPub as a publisher to the grid_coms topic.
 * Run initialise() method to send initial help message to the Grid.
 */
Mcp::Mcp() : rclcpp::Node("mcp")
{
    // Create a publisher to grid_coms topic
    m_gridComsPub = this->create_publisher<std_msgs::msg::String>(
        "grid_coms",
        rclcpp::QoS(10).transient_local().reliable());

    // m_user_coms_sub = this->create_subscription<std_msgs::msg::String>(
    //     "user_coms",
    //     10,
    //     std::bind(&Mcp::receive_user_coms, this, std::placeholders::_1)
    // );

    // m_set_process_server = this->create_service<grid_interfaces::srv::SetProcessId>(
    //     "set_process_id",
    //     std::bind(&Mcp::setProcessId, this, std::placeholders::_1, std::placeholders::_2)
    // );

    initialise();
}

/**
 * Send initial message to grid
 */
void Mcp::initialise() {
    auto initialContact = std_msgs::msg::String();
    initialContact.data = "Hello user, thank you for logging in.\n"
                        "A rogue program has invaded the Grid is corrupting sectors!\n"
                        "We need to deploy Tron to save us, but his process ID has been erased from my memory files.\n"
                        "Use admin task 3377 to find out the process ID for Tron, message me back with his ID and I will spin him up.\n";
    m_gridComsPub->publish(initialContact);
    RCLCPP_INFO(this->get_logger(), "Sent initial message");
}

// print user responses
// void Mcp::receive_user_coms(const std_msgs::msg::String &msg) {
//     RCLCPP_INFO(this->get_logger(), msg.data.c_str());
//     respond_user_coms();
// }

// void Mcp::respond_user_coms() {
//     auto userResponse = std_msgs::msg::String();
//     userResponse.data = "I don't understand! PLACEHOLDER";
//     m_grid_coms_pub->publish(userResponse);
// }

// // method called when user adds a process to the Grid
// void Mcp::setProcessId(const std::shared_ptr<grid_interfaces::srv::SetProcessId::Request> request,
//                   std::shared_ptr<grid_interfaces::srv::SetProcessId::Response> response)
// {
//     RCLCPP_INFO(this->get_logger(), "User has added process '%s' with ID '%i' to the Grid instantiables.", request->process_name.c_str(), request->process_id);
//     m_processIds[request->process_id] = request->process_name;
//     response->message = "Thank you user for using ENCOM mGrid, you process has been added.";
// }
