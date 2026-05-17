#include <random>

#include "Mcp.hpp"
#include "McpSubsystem.hpp"

/**
 * Create a node with label "mcp" and set m_gridComsPub as a publisher to the grid_coms topic.
 * Run initialise() method to send initial help message to the Grid.
 */
Mcp::Mcp() : rclcpp::Node("mcp")
{
    // Create a publisher to grid_coms topic
    m_gridComsPub = this->create_publisher<std_msgs::msg::String>(
        "mcp/grid_coms",
        rclcpp::QoS(10).transient_local().reliable()
    );

    // Create a client for Grid's add_process service and wait for it to be available
    m_addGridProcessClient = this->create_client<grid_interfaces::srv::AddProcess>("grid/add_process_request");
    while (!m_addGridProcessClient->wait_for_service(std::chrono::seconds(5)))
    {
        RCLCPP_INFO(this->get_logger(), "Waiting for Grid's add_process service timed out. Trying again.");
    }

    m_mcpComsSub = this->create_subscription<std_msgs::msg::String>(
        "user/mcp_coms",
        rclcpp::QoS(10).transient_local().reliable(),
        std::bind(&Mcp::handleMcpComsSub, this, std::placeholders::_1)
    );

    // Create a client for Grid's check_sector service and wait for it to be available
    m_checkSectorClient = this->create_client<grid_interfaces::srv::CheckSector>("grid/check_sector_request");
    while (!m_checkSectorClient->wait_for_service(std::chrono::seconds(5)))
    {
        RCLCPP_INFO(this->get_logger(), "Waiting for Grid's check_sector service timed out. Trying again.");
    }

    RCLCPP_INFO(this->get_logger(), "MCP started.");

    // Add Tron to the Grid and publish initial HELP message
    initialise();
}

/**
 * Add Tron to the Grid
 */
void Mcp::initialise()
{
    RCLCPP_INFO(this->get_logger(), "Adding Tron to the Grid.");

    // Make a request to the Grid's add_process service, sendInitialClientComs is the callback
    auto request = std::make_shared<grid_interfaces::srv::AddProcess::Request>();
    m_addGridProcessClient->async_send_request(
        request,
        std::bind(&Mcp::sendInitialClientComs, this, std::placeholders::_1)
    );
}

/**
 * Callback - post initial HELP message to the Grid after we have added Tron
 */
void Mcp::sendInitialClientComs(rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedFuture future)
{
    // Get process ID from Grid's add_process service future, construct initial HELP message and publish
    int32_t processPos = future.get()->process_pos;
    RCLCPP_INFO(this->get_logger(), "Grid has sent back process ID %d for Tron. Publishing HELP message to the Grid.", processPos);

    auto initialContact = std_msgs::msg::String();
    initialContact.data = m_mcpSubsystem.initialMessage(processPos);
    m_gridComsPub->publish(initialContact);
    RCLCPP_INFO(this->get_logger(), "Published HELP message to the Grid.");
}

/**
 * Receive coms from User when they either send back the new process position or request a reset.
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

    bool isInt = m_mcpSubsystem.checkInputInteger(msg.data);
    if(isInt)
    {
        auto request = std::make_shared<grid_interfaces::srv::CheckSector::Request>();
        request->sector = std::stoi(msg.data);
        m_checkSectorClient->async_send_request(
            request,
            std::bind(&Mcp::handleCheckSectorResponse, this, std::placeholders::_1)
        );
    }
    else
    {
        bool isResetRequest = m_mcpSubsystem.checkResetInput(msg.data);
        if(isResetRequest)
        {
            // Reset position and send in message to User
            auto request = std::make_shared<grid_interfaces::srv::AddProcess::Request>();
            m_addGridProcessClient->async_send_request(
                request,
                std::bind(&Mcp::sendResetPositionToUser, this, std::placeholders::_1)
            );
        }
        else
        {
            // Send message to user saying we didn't understand, please enter an integer or 'reset'
            auto msgToUser = std_msgs::msg::String();
            msgToUser.data = "Apologies User but I was unable to understand your previous message.\nPlease send the new process sector position as an integer or message me 'reset' if you wish to reset the test.\nEND OF LINE.";
            m_gridComsPub->publish(msgToUser);
        }
    }

}

/**
 * Callback for the Grid's check_sector service. Reset the position and return message to User based on if guess was correct.
 */
void Mcp::handleCheckSectorResponse(rclcpp::Client<grid_interfaces::srv::CheckSector>::SharedFuture future)
{
    auto msgToUser = std_msgs::msg::String();
    auto result = future.get();
    int32_t newSector = result->new_sector;
    if(result->correct)
    {
        msgToUser.data = "Great work User that is the correct sector, the process tracker works.\n"
                            "If you would like to test it again, I have redeployed Tron to sector "+ std::to_string(newSector) +
                            ".\nEND OF LINE\n";
    }
    else
    {
        msgToUser.data = "That doesn't seem like the correct sector User.\nI have redeployed Tron to sector " +
                            std::to_string(newSector) + " if you would like to test the tracker again.\nEND OF LINE";
    }
    m_gridComsPub->publish(msgToUser);
}

void Mcp::sendResetPositionToUser(rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedFuture future)
{
    auto msgToUser = std_msgs::msg::String();
    auto newSectorPos = future.get()->process_pos;
    msgToUser.data = "I have redeployed Tron to the Grid for you User. He is in sector " + std::to_string(newSectorPos) + ".\nEND OF LINE";
    m_gridComsPub->publish(msgToUser);
}