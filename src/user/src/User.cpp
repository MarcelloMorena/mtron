#include <string>
#include <vector>

#include "User.hpp"
#include "UserSubsystem.hpp"

/**
 * Create User as a node with label "user" and set m_getMessagesClient as a client to Grid's
 * get_messages service
 */
User::User() : rclcpp::Node("user")
{
    m_getMessagesClient = this->create_client<grid_interfaces::srv::GetMessages>("grid/get_messages_request");
    m_gridActionClient = rclcpp_action::create_client<grid_interfaces::action::TrackProcess>(
        this,
        "grid/track_process_request");
    m_mcpComsPub = this->create_publisher<std_msgs::msg::String>(
        "user/mcp_coms",
        rclcpp::QoS(10).transient_local().reliable()
    );

    // RCLCPP_INFO(this->get_logger(), "User started.");
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
        sendMessage();
        break;

    case 3:
        trackProcess();
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
    // RCLCPP_INFO(this->get_logger(), "Getting messages from the Grid.");

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
    // RCLCPP_INFO(this->get_logger(), "Message returned from the Grid.\n");

    // Get the message returned from the Grid and print to screen
    std::string message = future.get()->message;
    m_userSubsystem.printMessage(message);

    menu();
}

void User::sendMessage()
{
    auto message = std_msgs::msg::String();
    message.data = m_userSubsystem.messageUserInput();
    m_mcpComsPub->publish(message);
    std::cout << "Check messages for MCP response.\n";
    menu();
}

void User::trackProcess()
{
    // Terrible fix for bug where invalid sector guesses broke m_gridActionClient
    m_gridActionClient = rclcpp_action::create_client<grid_interfaces::action::TrackProcess>(
        this,
        "grid/track_process_request"
    );

    if(!m_gridActionClient->wait_for_action_server(std::chrono::seconds(5)))
    {
        RCLCPP_ERROR(this->get_logger(), "Grid action server not available after waiting.");
    }

    auto goalMsg = grid_interfaces::action::TrackProcess::Goal();
    int processPosGuess = m_userSubsystem.getProcessPos();
    goalMsg.process_pos = processPosGuess;
    // RCLCPP_INFO(this->get_logger(), "Requesting Grid to track process %d", processPosGuess);

    auto sendGoalOptions = rclcpp_action::Client<grid_interfaces::action::TrackProcess>::SendGoalOptions();
    sendGoalOptions.goal_response_callback = std::bind(&User::goalResponseCallback, this, std::placeholders::_1);
    sendGoalOptions.feedback_callback = std::bind(&User::feedbackCallback, this, std::placeholders::_1, std::placeholders::_2);
    sendGoalOptions.result_callback = std::bind(&User::resultCallback, this, std::placeholders::_1);
    m_gridActionClient->async_send_goal(goalMsg, sendGoalOptions);
}

void User::goalResponseCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::SharedPtr const &goalHandle)
{
    if(!goalHandle)
    {
        // RCLCPP_INFO(this->get_logger(), "Invalid process ID!");
        std::cout << "Invalid process position!\n";
        menu();
    }
    else
    {
        // RCLCPP_INFO(this->get_logger(), "Calling action on Grid");
    }
}

void User::feedbackCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::SharedPtr, std::shared_ptr<grid_interfaces::action::TrackProcess::Feedback const> const feedback)
{
    // RCLCPP_INFO(this->get_logger(), "FEEDBACK CALLED\n");
    // print feedback, sleep for 500ms, then erase with \r and spaces
    auto numProgress = feedback->partial_pos;
    std::cout << "Moving Tron - " << std::to_string(numProgress) << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout<<"\rMoving Tron -           \r";
}

void User::resultCallback(rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::WrappedResult const &result)
{
    // print final result, sleep for 500ms, then erase with \r and spaces
    auto finalNum = result.result->final_pos;
    std::cout << "Tron Moved - " << std::to_string(finalNum) << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout<<"\r                             \r";
    menu();
}
