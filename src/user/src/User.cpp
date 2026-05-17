#include <string>
#include <vector>

#include "User.hpp"
#include "UserSubsystem.hpp"

/**
 * Constructor - create a node with label "user" and create topic publisher and service and action
 * clients. Initialise the I/O loop.
 */
User::User() : rclcpp::Node("user")
{
    // Service client for grid/get_messages_request.
    m_getMessagesClient = this->create_client<grid_interfaces::srv::GetMessages>(
        "grid/get_messages_request"
    );

    // Action client for grid/track_process_request.
    m_gridActionClient = rclcpp_action::create_client<grid_interfaces::action::TrackProcess>(
        this,
        "grid/track_process_request");

    // Topic publisher (Mcp subscribes to this).
    m_mcpComsPub = this->create_publisher<std_msgs::msg::String>(
        "user/mcp_coms",
        rclcpp::QoS(10).transient_local().reliable()
    );

    // Initialise I/O loop.
    initialise();
}

/**
 * Print welcome message and begin menu loop.
 */
void User::initialise()
{
    m_userSubsystem.welcome();
    menu();
}

/**
 * Print menu options and parse user menu selection input. Call method relevant to user selection.
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

    case 4:
        RCLCPP_INFO(this->get_logger(), "Shutting down.");
        rclcpp::shutdown();
        return;

    default:
        break;
    }
}

/**
 * Menu item 1: Request messages from Grid using grid/get_messages_request service.
 */
void User::getMessages()
{
    // Send request to Grid service, set handleGetMessagesResponse() as callback.
    auto request = std::make_shared<grid_interfaces::srv::GetMessages::Request>();
    m_getMessagesClient->async_send_request(
        request,
        std::bind(&User::handleGetMessagesResponse, this, std::placeholders::_1)
    );

}

/**
 * Callback for grid/get_messages_request service's return value.
 * Print MCP's message returned from the Grid then restart the menu loop.
 *
 * @param future The return value from service grid/get_messages_request (MCP message string from the Grid)
 * 
 * @note Restarts the menu loop.
 */
void User::handleGetMessagesResponse(
    rclcpp::Client<grid_interfaces::srv::GetMessages>::SharedFuture future)
{
    // Get the message returned from the Grid and print to screen.
    std::string message = future.get()->message;
    m_userSubsystem.printMessage(message);
    menu();
}

/**
 * Menu item 2: Get user input and send as a message to the Grid with user/mcp_coms topic
 * 
 * @note Restarts the menu loop.
 */
void User::sendMessage()
{
    // Get user input and publish to Grid.
    auto message = std_msgs::msg::String();
    message.data = m_userSubsystem.messageUserInput();
    m_mcpComsPub->publish(message);
    std::cout << "Check messages for MCP response.\n";
    menu();
}

/**
 * Menu item 3: Use the Grid's process tracking function with grid/track_process_request action.
 * Get user input for process sector location and start the action.
 * 
 * @note Attaches goalResponseCallback, feedbackCallback, and resultCallback as the callbacks.
 * @note Recreates m_gridActionClient as a poor bug fix.
 */
void User::trackProcess()
{
    // Terrible fix for bug where invalid sector guesses broke m_gridActionClient
    m_gridActionClient = rclcpp_action::create_client<grid_interfaces::action::TrackProcess>(
        this,
        "grid/track_process_request"
    );

    // Ensure action server is active.
    if(!m_gridActionClient->wait_for_action_server(std::chrono::seconds(5)))
    {
        RCLCPP_ERROR(this->get_logger(), "Grid action server not available after waiting.");
    }

    // Create goal for action and attach user's input.
    auto goalMsg = grid_interfaces::action::TrackProcess::Goal();
    int processPosGuess = m_userSubsystem.getProcessPos();
    goalMsg.process_pos = processPosGuess;

    // Set the three callbacks for goal response, feedback, and result, then send the action request.
    auto sendGoalOptions = rclcpp_action::Client<grid_interfaces::action::TrackProcess>::SendGoalOptions();
    sendGoalOptions.goal_response_callback = std::bind(&User::goalResponseCallback, this, std::placeholders::_1);
    sendGoalOptions.feedback_callback = std::bind(&User::feedbackCallback, this, std::placeholders::_1, std::placeholders::_2);
    sendGoalOptions.result_callback = std::bind(&User::resultCallback, this, std::placeholders::_1);
    m_gridActionClient->async_send_goal(goalMsg, sendGoalOptions);
}

/**
 * Callback for grid/track_process_request action's goal response.
 * If the goal was rejected due to invalid input, restart the manu loop.
 * 
 * @param goalHandle The goal handle from the Grid action server.
 * 
 * @note Restarts the menu loop if goal was rejected.
 */
void User::goalResponseCallback(
    rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::SharedPtr const& goalHandle)
{
    // Restart menu loop if goal was rejected.
    if(!goalHandle)
    {
        std::cout << "Invalid process position!\n";
        menu();
    }
}

/**
 * Called as feedback comes in from grid/track_process_request.
 * Show the current sector digit on screen for 500ms then clear it.
 * 
 * @param goalHandle The goal handle from the Grid action server.
 * @param feedback The feedback from the Grid action server. Contains a single digit of the
 *                  sector location as an int32_t.
 */
void User::feedbackCallback(
    rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::SharedPtr const& goalHandle,
    std::shared_ptr<grid_interfaces::action::TrackProcess::Feedback const> const feedback)
{
    // Prevent warning about unused variable.
    (void)goalHandle;

    // Print feedback, sleep for 500ms, then erase with \r and spaces.
    auto numProgress = feedback->partial_pos;
    std::cout << "Moving Tron - " << std::to_string(numProgress) << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout<<"\rMoving Tron -           \r";
}

/**
 * Called when grid/track_process_request action finishes. Print the final sector location to screen
 * one last time for 500ms before clearing it.
 * 
 * @param result The final result from the Grid action server. Contains the full sector location as
 *                  an int32_t
 * 
 * @note Restarts the menu loop.
 */
void User::resultCallback(
    rclcpp_action::ClientGoalHandle<grid_interfaces::action::TrackProcess>::WrappedResult const& result)
{
    // Print final result, sleep for 500ms, then erase with \r and spaces
    auto finalNum = result.result->final_pos;
    std::cout << "Tron Moved - " << std::to_string(finalNum) << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout<<"\r                             \r";
    menu();
}
