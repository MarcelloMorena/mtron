#include "Mcp.hpp"
#include <chrono>

// constructor creates a node with label "mcp", adds it as a publisher and subscriber to two topics
Mcp::Mcp() : rclcpp::Node("mcp") {
    m_mcp_coms_pub = this->create_publisher<std_msgs::msg::String>("mcp_coms", 10);
    m_user_coms_sub = this->create_subscription<std_msgs::msg::String>("user_coms", 10,
        std::bind(&Mcp::receive_user_coms, this, std::placeholders::_1));
    m_set_process_server = this->create_service<grid_interfaces::srv::SetProcessId>("set_process_id", std::bind(&Mcp::setProcessId, this, std::placeholders::_1, std::placeholders::_2));

    m_initial_timer = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&Mcp::initialise, this));
}

// initial message to user
void Mcp::initialise() {
    m_initial_timer->cancel();
    auto initialContact = std_msgs::msg::String();
    initialContact.data = "Hello user, thank you for logging in."
                            " A rogue program has invaded the Grid and disabled my Master Control"
                            " access! We need to deploy Tron to save us, but its process ID has"
                            " been corrupted. Can you use the ‘ros2 service’ command to scan my"
                            " Master Control services for anything helpful? Reply back when you"
                            " have Tron’s process ID and I will spin him up to fight the virus!";
    m_mcp_coms_pub->publish(initialContact);
}

// print user responses
void Mcp::receive_user_coms(const std_msgs::msg::String &msg) const {
    RCLCPP_INFO(this->get_logger(), msg.data.c_str());
    respond_user_coms();
}

void Mcp::respond_user_coms() const {
    auto userResponse = std_msgs::msg::String();
    userResponse.data = "I don't understand! PLACEHOLDER";
    m_mcp_coms_pub->publish(userResponse);
}

// method called when user adds a process to the Grid
void Mcp::setProcessId(const std::shared_ptr<grid_interfaces::srv::SetProcessId::Request> request,
                  std::shared_ptr<grid_interfaces::srv::SetProcessId::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "User has added process '%s' with ID '%i' to the Grid instantiables.", request->process_name.c_str(), request->process_id);
    m_processIds[request->process_id] = request->process_name;
    response->message = "Thank you user for using ENCOM mGrid, you process has been added.";
}
