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
    RCLCPP_INFO(this->get_logger(), "Getting messages from the Grid.");

    // Send request to Grid get_messages services, set handleGetMessagesResponse as callback
    auto request = std::make_shared<grid_interfaces::srv::GetMessages::Request>();
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

    // Get the messages returned from the Grid and print to screen
    std::vector<std::string> messages = future.get()->messages;
    m_userSubsystem.printMessages(messages);

    menu();
}