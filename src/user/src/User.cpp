#include "User.hpp"
#include<string>

using std::placeholders::_1;

User::User() : rclcpp::Node("user") {
  m_user_coms_pub = this->create_publisher<std_msgs::msg::String>("user_coms", 10);
  m_mcp_coms_sub = this->create_subscription<std_msgs::msg::String>("mcp_coms", 10,
    std::bind(&User::receive_mcp_coms, this, _1));
}

void User::initialise() const {
  std::string input;
  std::getline(std::cin, input);
  auto response = std_msgs::msg::String();
  response.data = input;
  m_user_coms_pub->publish(response);
}

void User::receive_mcp_coms(const std_msgs::msg::String &msg) const {
  RCLCPP_INFO(this->get_logger(), msg.data.c_str());
  initialise();
}
