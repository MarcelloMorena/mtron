#include <random>

#include "Mcp.hpp"
#include "McpSubsystem.hpp"

/**
 * Create a node with label "mcp" create publisher, subscriber, and service clients.
 * Sends initial help message to the Grid.
 */
Mcp::Mcp() : rclcpp::Node("mcp")
{
    // Create a publisher to mcp/grid_coms topic.
    m_gridComsPub = this->create_publisher<std_msgs::msg::String>(
        "mcp/grid_coms",
        rclcpp::QoS(10).transient_local().reliable()
    );

    // Create a client for grid/add_process_request service and wait for it to be available.
    m_addGridProcessClient = this->create_client<grid_interfaces::srv::AddProcess>("grid/add_process_request");
    while (!m_addGridProcessClient->wait_for_service(std::chrono::seconds(5)))
    {
        RCLCPP_INFO(this->get_logger(), "Waiting for Grid's add_process service timed out. Trying again.");
    }

    // Create subscription to user/mcp_coms.
    m_mcpComsSub = this->create_subscription<std_msgs::msg::String>(
        "user/mcp_coms",
        rclcpp::QoS(10).transient_local().reliable(),
        std::bind(&Mcp::handleMcpComsSub, this, std::placeholders::_1)
    );

    // Create a client for grid/check_sector_request service and wait for it to be available.
    m_checkSectorClient = this->create_client<grid_interfaces::srv::CheckSector>("grid/check_sector_request");
    while (!m_checkSectorClient->wait_for_service(std::chrono::seconds(5)))
    {
        RCLCPP_INFO(this->get_logger(), "Waiting for Grid's check_sector service timed out. Trying again.");
    }

    RCLCPP_INFO(this->get_logger(), "MCP started.");

    // Add Tron process to the Grid and publish initial HELP message.
    initialise();
}

/**
 * Add Tron to the Grid using grid/add_process_request service.
 * 
 * @note Sets sendInitialClientComs() as callback.
 */
void Mcp::initialise()
{
    RCLCPP_INFO(this->get_logger(), "Adding Tron to the Grid.");

    // Make request and send it, set sendInitialClientComs as the callback.
    auto request = std::make_shared<grid_interfaces::srv::AddProcess::Request>();
    m_addGridProcessClient->async_send_request(
        request,
        std::bind(&Mcp::sendInitialClientComs, this, std::placeholders::_1)
    );
}

/**
 * Callback for grid/add_process_request - Post initial HELP message to the Grid using the
 * process sector position we have received from the service.
 *
 * @param future Return value from the service.
 *
 * @note Publishes HELP message to mcp/grid_coms topic.
 */
void Mcp::sendInitialClientComs(rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedFuture future)
{
    // Get process ID from the service's returned future.
    int32_t processPos = future.get()->process_pos;
    RCLCPP_INFO(this->get_logger(), "Grid has sent back process ID %d for Tron. Publishing HELP message to the Grid.", processPos);

    // Construct HELP message and publish.
    auto initialContact = std_msgs::msg::String();
    initialContact.data = m_mcpSubsystem.initialMessage(processPos);
    m_gridComsPub->publish(initialContact);
    RCLCPP_INFO(this->get_logger(), "Published HELP message to the Grid.");
}

/**
 * Callback for user/mcp_coms subscriber.
 * Handle coms from User when they either send back the new process position or request a reset.
 * If user requests a reset, reset the process sector location using Grid service grid/add_process_request.
 * If user enters process position, check if correct using Grid service grid/check_sector_request
 * and return success or failure message.
 * If user enters anything else, send invalid input message.
 *
 * @param msg The message from the User.
 * 
 * @note sets handleCheckSectorResponse() and sendResetPositionToUser() as callbacks.
 */
void Mcp::handleMcpComsSub(std_msgs::msg::String const& msg)
{
    // check if user input is string
    // if string check if it says 'reset', if so reset if not send message 'don't understand'
    // if integer, check if correct position, if so return congratulations message
    // if not correct position return invalid message and reset

    // call subsystem method to check if integer or string
    // if integer call check position service on Grid
    // if string check if reset, if so call reset on grid

    // Check if input is integer, if so it is a sector location guess and we check if it is correct.
    bool isInt = m_mcpSubsystem.checkInputInteger(msg.data);
    if(isInt)
    {
        // Convert to int and use grid/check_sector_request service to check if correct.
        auto request = std::make_shared<grid_interfaces::srv::CheckSector::Request>();
        request->sector = std::stoi(msg.data);
        m_checkSectorClient->async_send_request(
            request,
            std::bind(&Mcp::handleCheckSectorResponse, this, std::placeholders::_1)
        );
    }

    // If input is a string, check if it is a 'reset' request. If so reset the sector position and
    // return to user. If not, send invalid input message.
    else
    {
        bool isResetRequest = m_mcpSubsystem.checkResetInput(msg.data);
        if(isResetRequest)
        {
            // Reset position and send as message to User.
            auto request = std::make_shared<grid_interfaces::srv::AddProcess::Request>();
            m_addGridProcessClient->async_send_request(
                request,
                std::bind(&Mcp::sendResetPositionToUser, this, std::placeholders::_1)
            );
        }
        else
        {
            // Send message to user saying we didn't understand, please enter an integer or 'reset'.
            auto msgToUser = std_msgs::msg::String();
            msgToUser.data = "Apologies User but I was unable to understand your previous message.\n"
                                "Please send the new process sector position as an integer or "
                                "message me 'reset' if you wish to reset the test.\nEND OF LINE.";
            m_gridComsPub->publish(msgToUser);
        }
    }

}

/**
 * Callback for grid/check_sector_request service.
 * Reset the sector position and send a message to User based on if guess was correct or not.
 *
 * @param future Return value from grid/check_sector_request service.
 *
 * @note Publishes to mcp/grid_coms topic.
 */
void Mcp::handleCheckSectorResponse(rclcpp::Client<grid_interfaces::srv::CheckSector>::SharedFuture future)
{
    auto srvReturn = future.get();
    int32_t newSectorPos = srvReturn->new_sector;
    auto msgToUser = std_msgs::msg::String();
    if(srvReturn->correct)
    {
        msgToUser.data = "Great work User that is the correct sector, the process tracker works.\n"
                            "If you would like to test it again, I have redeployed Tron to sector "+ std::to_string(newSectorPos) +
                            ".\nEND OF LINE\n";
    }
    else
    {
        msgToUser.data = "That doesn't seem like the correct sector User.\nI have redeployed Tron to sector " +
                            std::to_string(newSectorPos) + " if you would like to test the tracker again.\nEND OF LINE";
    }
    m_gridComsPub->publish(msgToUser);
}

/**
 * Callback for grid/add_process_request service.
 * User has requested process sector position reset, send back the new position received from Grid.
 *
 * @param future Return value from grid/add_process_request.
 */
void Mcp::sendResetPositionToUser(rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedFuture future)
{
    auto msgToUser = std_msgs::msg::String();
    auto newSectorPos = future.get()->process_pos;
    msgToUser.data = "I have redeployed Tron to the Grid for you User. He is in sector " + 
                        std::to_string(newSectorPos) + ".\nEND OF LINE";
    m_gridComsPub->publish(msgToUser);
}
