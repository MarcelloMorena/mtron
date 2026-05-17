#include "Grid.hpp"

/**
 * Create a node with label "grid", set m_gridComsSub as a subscription to grid_coms topic,
 * set m_getMessagesService as a service called get_messages linked to the getMessagesService method
 */
Grid::Grid() : rclcpp::Node("grid")
{
    // Create a subscription to grid_coms topic
    m_gridComsSub = this->create_subscription<std_msgs::msg::String>(
        "mcp/grid_coms",
        rclcpp::QoS(10).transient_local().reliable(),
        std::bind(&Grid::receiveGridComs, this, std::placeholders::_1)
    );

    // Create get_messages service (for User)
    m_getMessagesService = this->create_service<grid_interfaces::srv::GetMessages>(
        "grid/get_messages_request",
        std::bind(&Grid::getMessagesService, this, std::placeholders::_1, std::placeholders::_2)
    );

    // Create add_process service (for Mcp)
    m_addProcessService = this->create_service<grid_interfaces::srv::AddProcess>(
        "grid/add_process_request",
        std::bind(&Grid::addProcessService, this, std::placeholders::_1, std::placeholders::_2)
    );

    // Create track_process action (for User)
    m_trackProcessActionServer = rclcpp_action::create_server<grid_interfaces::action::TrackProcess>(
        this,
        "grid/track_process_request",
        std::bind(&Grid::handleGoal, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Grid::handleCancel, this, std::placeholders::_1),
        std::bind(&Grid::handleAccepted, this, std::placeholders::_1)
    );

    // Create check_sector service (for Mcp)
    m_checkSectorService = this->create_service<grid_interfaces::srv::CheckSector>(
        "grid/check_sector_request",
        std::bind(&Grid::checkSectorService, this, std::placeholders::_1, std::placeholders::_2)
    );

    RCLCPP_INFO(this->get_logger(), "Grid started.");
}

/**
 * Callback for messages posted to grid_coms topic
 */
void Grid::receiveGridComs(std_msgs::msg::String::SharedPtr const msg) {
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
    response->message = m_gridSubsystem.getMessage();
}

/**
 * Callback for request made by Mcp to add_process service.
 * Adds process to Grid with a randomly generated pos
 */
void Grid::addProcessService(const std::shared_ptr<grid_interfaces::srv::AddProcess::Request> request,
                                std::shared_ptr<grid_interfaces::srv::AddProcess::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "MCP requested to add process.");
    
    (void)request;
    // Add process to grid and get the randomly generated position
    int32_t processPos = m_gridSubsystem.addProcess();

    RCLCPP_INFO(this->get_logger(), "Added process with position %d.", processPos);

    // Return processId to Mcp
    response->process_pos = processPos;
}

rclcpp_action::GoalResponse Grid::handleGoal(
    rclcpp_action::GoalUUID const& uuid,
    std::shared_ptr<grid_interfaces::action::TrackProcess::Goal const> goal)
{
    (void)uuid;
    if(m_gridSubsystem.processExists(goal->process_pos))
    {
        RCLCPP_INFO(this->get_logger(), "Received valid task request for process position %d, accepting.", goal->process_pos);
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    RCLCPP_INFO(this->get_logger(), "Received invalid task request for process position %d, rejecting.", goal->process_pos);
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
    rclcpp::Rate loopRate(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto const goal = goalHandle->get_goal();
    auto feedback = std::make_shared<grid_interfaces::action::TrackProcess::Feedback>();
    auto result = std::make_shared<grid_interfaces::action::TrackProcess::Result>();

    for (int i = 0; i < 4; i++)
    {
        // If the action is cancelled finalise the path early (move tron) and return the path
        if(goalHandle->is_canceling())
        {
            result->final_pos = m_gridSubsystem.finalisePath();
            goalHandle->canceled(result);
            return;
        }

        int32_t tronPath = m_gridSubsystem.pathTrace();
        RCLCPP_INFO(this->get_logger(), "Moving Tron - %d", tronPath);

        // Next position segment
        feedback->partial_pos = tronPath;

        // Publish feedback
        goalHandle->publish_feedback(feedback);
        loopRate.sleep();
    }

    if(rclcpp::ok())
    {
        int32_t finalSectorPos = m_gridSubsystem.finalisePath();
        RCLCPP_INFO(this->get_logger(), "Final Tron position - %d", finalSectorPos);
        result->final_pos = finalSectorPos;
        goalHandle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Action successful");
    }
}

void Grid::checkSectorService(const std::shared_ptr<grid_interfaces::srv::CheckSector::Request> request,
                                std::shared_ptr<grid_interfaces::srv::CheckSector::Response> response)
{
    response->correct = m_gridSubsystem.checkSector(request->sector);
    response->new_sector = m_gridSubsystem.addProcess();
}
