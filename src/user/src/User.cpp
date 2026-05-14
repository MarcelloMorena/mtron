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
    m_gridActionClient = rclcpp_action::create_client<grid_interfaces::action::PerformTask>(
        this,
        "perform_task"
    );
    
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
        // track process id
        trackProcess();
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

void User::trackProcess()
{
    if(!m_gridActionClient->wait_for_action_server(std::chrono::seconds(5)))
    {
        RCLCPP_ERROR(this->get_logger(), "Grid action server not available after waiting.");
    }

    auto goalMsg = grid_interfaces::action::PerformTask::Goal();
    goalMsg.process_id = m_userSubsystem.getProcessId();

    auto sendGoalOptions = rclcpp_action::Client<grid_interfaces::action::PerformTask>::SendGoalOptions();
    sendGoalOptions.goal_response_callback = std::bind(&User::goalResponseCallback, this, std::placeholders::_1);
    sendGoalOptions.feedback_callback = std::bind(&User::feedbackCallback, this, std::placeholders::_1, std::placeholders::_2);
    sendGoalOptions.result_callback = std::bind(&User::resultCallback, this, std::placeholders::_1);
    m_gridActionClient->async_send_goal(goalMsg, sendGoalOptions);
}

void User::goalResponseCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::PerformTask>::SharedPtr const &goalHandle)
{
    if(!goalHandle)
    {
        RCLCPP_INFO(this->get_logger(), "Invalid process ID!");
        menu();
    }
    else
    {
        RCLCPP_INFO(this->get_logger(), "Calling action on Grid");
    }
}

void User::feedbackCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::PerformTask>::SharedPtr, std::shared_ptr<grid_interfaces::action::PerformTask::Feedback const> const feedback)
{
    // print feedback, sleep for 500ms, then erase with \r and spaces
}

void User::resultCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::PerformTask>::WrappedResult const &result)
{
    // print final result, sleep for 500ms, then erase with \r and spaces
}
