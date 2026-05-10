#include "Mcp.hpp"
#include <string>

using std::placeholders::_1;

Mcp::Mcp() : rclcpp::Node("mcp") {
    m_mcp_coms_pub = this->create_publisher<std_msgs::msg::String>("mcp_coms", 10);
    m_user_coms_sub = this->create_subscription<std_msgs::msg::String>("user_coms", 10,
        std::bind(&Mcp::receive_user_coms, this, _1));
    initialise();
}

void Mcp::initialise() const {
    auto initialContact = std_msgs::msg::String();
    initialContact.data = "Hello user, thank you for logging in.\nPlease set the process ID for mTron and tell me, then enable my instantiation rights so I can deploy him to save the Grid!";
    m_mcp_coms_pub->publish(initialContact);
}

void Mcp::receive_user_coms(const std_msgs::msg::String &msg) const {
    RCLCPP_INFO(this->get_logger(), msg.data.c_str());
}
