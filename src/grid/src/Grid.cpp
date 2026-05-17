#include "Grid.hpp"

/**
 * Create a node with label "grid" and create topic subscription and service and action servers.
 */
Grid::Grid() : rclcpp::Node("grid")
{
    // Create a subscription to mcp/grid_coms topic.
    m_gridComsSub = this->create_subscription<std_msgs::msg::String>(
        "mcp/grid_coms",
        rclcpp::QoS(10).transient_local().reliable(),
        std::bind(&Grid::receiveGridComs, this, std::placeholders::_1)
    );

    // Create grid/get_messages_request service server (used by User).
    m_getMessagesService = this->create_service<grid_interfaces::srv::GetMessages>(
        "grid/get_messages_request",
        std::bind(&Grid::getMessagesService, this, std::placeholders::_1, std::placeholders::_2)
    );

    // Create grid/add_process_request service server (used by Mcp).
    m_addProcessService = this->create_service<grid_interfaces::srv::AddProcess>(
        "grid/add_process_request",
        std::bind(&Grid::addProcessService, this, std::placeholders::_1, std::placeholders::_2)
    );

    // Create grid/track_process_request action server (used by User).
    m_trackProcessActionServer = rclcpp_action::create_server<grid_interfaces::action::TrackProcess>(
        this,
        "grid/track_process_request",
        std::bind(&Grid::handleGoal, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Grid::handleCancel, this, std::placeholders::_1),
        std::bind(&Grid::handleAccepted, this, std::placeholders::_1)
    );

    // Create grid/check_sector_request service server (used by Mcp).
    m_checkSectorService = this->create_service<grid_interfaces::srv::CheckSector>(
        "grid/check_sector_request",
        std::bind(&Grid::checkSectorService, this, std::placeholders::_1, std::placeholders::_2)
    );

    RCLCPP_INFO(this->get_logger(), "Grid started.");
}

/**
 * Callback for messages posted to mcp/grid_coms topic. Saves message to Grid for User to request later.
 * 
 * @param msg Message from MCP to save to Grid.
 */
void Grid::receiveGridComs(std_msgs::msg::String::SharedPtr const msg) {
    RCLCPP_INFO(this->get_logger(), "Received coms from MCP:\n\n%s", msg->data.c_str());

    // Save message.
    m_gridSubsystem.addMessage(msg->data);
}

/**
 * Callback for requests made to the grid/get_messages_request service. Return saved MCP message to User.
 * 
 * @param request This is empty because this service is a trigger type with no input.
 * @param response The return, the message goes in here.
 */
void Grid::getMessagesService(std::shared_ptr<grid_interfaces::srv::GetMessages::Request> const request,
                                std::shared_ptr<grid_interfaces::srv::GetMessages::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "User requested messages, sending.");

    // Disable compiler warnings for unused request parameter.
    (void)request;
    // Return saved message.
    response->message = m_gridSubsystem.getMessage();
}

/**
 * Callback for request made by MCP to grid/add_process_request service.
 * Randomly generated a new sector location, save to Grid and return to MCP.
 * 
 * @param request This is empty because this service is a trigger type with no input.
 * @param response The return, the newly generated sector location goes in here.
 */
void Grid::addProcessService(std::shared_ptr<grid_interfaces::srv::AddProcess::Request> const request,
                                std::shared_ptr<grid_interfaces::srv::AddProcess::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "MCP requested to add process.");
    
    // Disable compiler warnings for unused request parameter.
    (void)request;

    // Get the randomly generated sector position.
    int32_t processPos = m_gridSubsystem.addProcess();

    RCLCPP_INFO(this->get_logger(), "Added process with position %d.", processPos);

    // Return processId to MCP.
    response->process_pos = processPos;
}

/**
 * Handle request for grid/track_process_request action.
 * Check if input process position is correct, if so accept the goal else reject.
 */
rclcpp_action::GoalResponse Grid::handleGoal(
    rclcpp_action::GoalUUID const& uuid,
    std::shared_ptr<grid_interfaces::action::TrackProcess::Goal const> goal)
{
    // Disable compiler warnings about unused parameter.
    (void)uuid;

    // Check if input process sector position is correct, if so accept else reject.
    if(m_gridSubsystem.checkSector(goal->process_pos))
    {
        RCLCPP_INFO(this->get_logger(), "Received valid task request for process position %d, accepting.", goal->process_pos);
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    RCLCPP_INFO(this->get_logger(), "Received invalid task request for process position %d, rejecting.", goal->process_pos);
    return rclcpp_action::GoalResponse::REJECT;
}

/**
 * Accept all cancellation requests for grid/track_process_request action.
 */
rclcpp_action::CancelResponse Grid::handleCancel(
    std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle)
{
    RCLCPP_INFO(this->get_logger(), "Received request to cancel active process.");
    (void)goalHandle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

/**
 * If grid/track_process_request action request is accepted, call trackProcess() in another thread.
 */
void Grid::handleAccepted(
    std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle)
{
    std::thread{std::bind(&Grid::trackProcess, this, std::placeholders::_1), goalHandle}.detach();
}

/**
 * Handles accepted action requests to grid/track_process_request.
 * Slowly generates 4 random digits one at a time and returns them to the action caller.
 * At the end returns the full 4 digit sector location to the action caller and saves it as the new
 * position.
 */
void Grid::trackProcess(std::shared_ptr<rclcpp_action::ServerGoalHandle<grid_interfaces::action::TrackProcess>> const goalHandle)
{
    // Set the loop to occur once a second.
    // Sleep for .1 second at the start to (poorly) resolve race condition.
    // Get the feedback and result references to put values into later.
    rclcpp::Rate loopRate(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto feedback = std::make_shared<grid_interfaces::action::TrackProcess::Feedback>();
    auto result = std::make_shared<grid_interfaces::action::TrackProcess::Result>();

    // Randomly generate 4 digits between 1-9 once a second.
    for (int i = 0; i < 4; i++)
    {
        // If the action is cancelled finalise the path early and return the path.
        if(goalHandle->is_canceling())
        {
            result->final_pos = m_gridSubsystem.finalisePath();
            goalHandle->canceled(result);
            return;
        }

        // Get the next digit and add to feedback.
        int32_t tronPath = m_gridSubsystem.pathTrace();
        RCLCPP_INFO(this->get_logger(), "Moving Tron - %d", tronPath);
        feedback->partial_pos = tronPath;

        // Publish feedback and sleep.
        goalHandle->publish_feedback(feedback);
        loopRate.sleep();
    }

    // Once all 4 digits are generated return the full sector location as final resul.
    if(rclcpp::ok())
    {
        int32_t finalSectorPos = m_gridSubsystem.finalisePath();
        RCLCPP_INFO(this->get_logger(), "Final Tron position - %d", finalSectorPos);
        result->final_pos = finalSectorPos;
        goalHandle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Action successful");
    }
}

/**
 * Callback for grid/check_sector_request service.
 * Check if input int is correct sector location, and provide new sector location.
 * 
 * @param request Contains the input sector location int from the User.
 * @param response Return whether the input is the correct location or not, and return new
 *                  sector location.
 */
void Grid::checkSectorService(const std::shared_ptr<grid_interfaces::srv::CheckSector::Request> request,
                                std::shared_ptr<grid_interfaces::srv::CheckSector::Response> response)
{
    // Return whether input sector is correct.
    response->correct = m_gridSubsystem.checkSector(request->sector);

    // Return newly generated sector location.
    response->new_sector = m_gridSubsystem.addProcess();
}
