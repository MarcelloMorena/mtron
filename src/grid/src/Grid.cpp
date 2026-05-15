#include "Grid.hpp"

/**
 * Create a node with label "grid", set m_gridComsSub as a subscription to grid_coms topic,
 * set m_getMessagesService as a service called get_messages linked to the getMessagesService method
 */
Grid::Grid() : rclcpp::Node("grid")
{
    // Create a subscription to grid_coms topic
    m_gridComsSub = this->create_subscription<std_msgs::msg::String>(
        "grid_coms",
        rclcpp::QoS(10).transient_local().reliable(),
        std::bind(&Grid::receiveGridComs, this, std::placeholders::_1)
    );

    // Create get_messages service (for User)
    m_getMessagesService = this->create_service<grid_interfaces::srv::GetMessages>(
        "get_messages",
        std::bind(&Grid::getMessagesService, this, std::placeholders::_1, std::placeholders::_2)
    );

    // Create add_process service (for Mcp)
    m_addProcessService = this->create_service<grid_interfaces::srv::AddProcess>(
        "add_process",
        std::bind(&Grid::addProcessService, this, std::placeholders::_1, std::placeholders::_2)
    );

    // Create track_process action (for User)
    m_trackProcessActionServer = rclcpp_action::create_server<grid_interfaces::action::TrackProcess>(
        this,
        "track_process",
        std::bind(&Grid::handleGoal, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Grid::handleCancel, this, std::placeholders::_1),
        std::bind(&Grid::handleAccepted, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "Grid started.");
}

/**
 * Callback for messages posted to grid_coms topic
 */
void Grid::receiveGridComs(const std_msgs::msg::String::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), "Received coms from MCP:\n\n%s", msg->data.c_str());

    // Add to messages vector
    m_gridSubsystem.addMessage(msg->data);
}

/**
 * Callback for requests made to the get_messages service. Return messages
 */
void Grid::getMessagesService(std::shared_ptr<grid_interfaces::srv::GetMessages::Request> request,
                                std::shared_ptr<grid_interfaces::srv::GetMessages::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "User requested messages, sending.");

    // Disable compiler warnings for unused request variable
    (void)request;
    response->messages = m_gridSubsystem.getMessages();
}

/**
 * Callback for request made by Mcp to add_process service.
 * Adds input process name to processes list with a randomly generated ID (no ID doubles)
 */
void Grid::addProcessService(const std::shared_ptr<grid_interfaces::srv::AddProcess::Request> request,
                                std::shared_ptr<grid_interfaces::srv::AddProcess::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "MCP requested to add process %s.", request->process_name.c_str());
    
    // Add process name to grid and get the randomly generated ID
    int32_t processId = m_gridSubsystem.addProcess(request->process_name);

    RCLCPP_INFO(this->get_logger(), "Added process %s with ID %d.", request->process_name.c_str(), processId);

    // Return processId to Mcp
    response->process_id = processId;
}

rclcpp_action::GoalResponse Grid::handleGoal(
    rclcpp_action::GoalUUID const& uuid,
    std::shared_ptr<grid_interfaces::action::TrackProcess::Goal const> goal)
{
    (void)uuid;
    if(m_gridSubsystem.processExists(goal->process_id))
    {
        RCLCPP_INFO(this->get_logger(), "Received valid task request for process ID %d, accepting.", goal->process_id);
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    RCLCPP_INFO(this->get_logger(), "Received invalid task request for process ID %d, rejecting.", goal->process_id);
    return rclcpp_action::GoalResponse::REJECT;
}

rclcpp_action::CancelResponse Grid::handleCancel(
    std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle)
{
    RCLCPP_INFO(this->get_logger(), "Received request to cancel active process.");
    (void)goalHandle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

void Grid::handleAccepted(
    std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle)
{
    std::thread{std::bind(&Grid::trackProcess, this, std::placeholders::_1), goalHandle}.detach();
}

void Grid::trackProcess(std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle)
{
    rclcpp::Rate loopRate(2);
    auto const goal = goalHandle->get_goal();
    auto feedback = std::make_shared<grid_interfaces::action::TrackProcess::Feedback>();
    auto result = std::make_shared<grid_interfaces::action::TrackProcess::Result>();
    auto processId = goal->process_id;

    for (int i = 0; i < 3; i++)
    {
        // If the action is cancelled finalise the path early (move tron) and return the path
        if(goalHandle->is_canceling())
        {
            result->final_path = m_gridSubsystem.finalisePath(processId);
            goalHandle->canceled(result);
            return;
        }
        // Next path segment
        feedback->partial_path = m_gridSubsystem.pathTrace();

        // Publish feedback
        goalHandle->publish_feedback(feedback);
        loopRate.sleep();
    }

    if(rclcpp::ok())
    {
        result->final_path = m_gridSubsystem.finalisePath(processId);
        goalHandle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Action successful");
    }
}
