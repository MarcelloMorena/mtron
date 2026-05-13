#include <string>
#include <vector>

#include "User.hpp"
#include "UserSubsystem.hpp"

//#include <chrono>

/**
 * Create User as a node with label "user" and set m_getMessagesClient as a client to Grid's
 * get_messages service
 */
User::User() : rclcpp::Node("user")
{
    // m_user_coms_pub = this->create_publisher<std_msgs::msg::String>("user_coms", 10);
    // m_mcp_coms_sub = this->create_subscription<std_msgs::msg::String>("mcp_coms", 10,
    //                         std::bind(&User::receive_mcp_coms, this, std::placeholders::_1));

    m_getMessagesClient = this->create_client<grid_interfaces::srv::GetMessages>("get_messages");
    RCLCPP_INFO(this->get_logger(), "User started.");
    initialise();
}

/**
 * Print welcome message and begin menu loop
 */
void User::initialise()
{
    m_userSubsystem.welcome();
    menu();
}

/**
 * Print menu options and get user input
 */
void User::menu()
{
    int menuSel = m_userSubsystem.menu();
    switch (menuSel)
    {
    case 1:
        getMessages();
        break;

    case 2:
        // publish message to mcp_coms
        break;

    case 3:
        // using task id do something to grid
        break;

    default:
        break;
    }
}

/**
 * Menu item 1: Request messages from Grid using get_messages service
 */
void User::getMessages()
{
    auto request = std::make_shared<grid_interfaces::srv::GetMessages::Request>();
    RCLCPP_INFO(this->get_logger(), "Getting messages from the Grid.");

    m_getMessagesClient->async_send_request(
        request,
        std::bind(&User::handleGetMessagesResponse, this, std::placeholders::_1)
    );

}

/**
 * Callback for get_message service's return value. Print messages
 */
void User::handleGetMessagesResponse(rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedFuture future)
{
    RCLCPP_INFO(this->get_logger(), "Messages returned from the Grid.");

    std::vector<std::string> messages = future.get()->messages;
    m_userSubsystem.printMessages(messages);

    // auto response = future.get();
    // for (const auto &msg : response->messages)
    // {
    //     std::cout << msg << "\n";
    // }
    menu();
}

// void User::reply() const
// {
//     std::string input;
//     std::getline(std::cin, input);
//     auto response = std_msgs::msg::String();
//     response.data = input;
//     m_user_coms_pub->publish(response);
// }

// void User::receive_mcp_coms(const std_msgs::msg::String &msg) const
// {
//     RCLCPP_INFO(this->get_logger(), msg.data.c_str());
//     reply();
// }
