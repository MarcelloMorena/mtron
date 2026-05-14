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
        "grid_coms",
        rclcpp::QoS(10).transient_local().reliable()
    );

    // Create a client for Grid's add_process service and wait for it to be available
    m_addGridProcessClient = this->create_client<grid_interfaces::srv::AddProcess>("add_process");
    while (!m_addGridProcessClient->wait_for_service(std::chrono::seconds(5)))
    {
        RCLCPP_INFO(this->get_logger(), "Waiting for Grid's add_process service timed out. Trying again.");
    }
    
    RCLCPP_INFO(this->get_logger(), "MCP started.");

    // Add process_search.task to the Grid and publish initial HELP message
    initialise();
}

/**
 * Add process_search.task to the Grid
 */
void Mcp::initialise()
{
    RCLCPP_INFO(this->get_logger(), "Add process_search.task to the Grid.");

    // Set request.process_name and make a request to the Grid's add_process service.
    // sendInitialClientComs is the callback
    auto request = std::make_shared<grid_interfaces::srv::AddProcess::Request>();
    request->process_name = "process_search.task";
    m_addGridProcessClient->async_send_request(
        request,
        std::bind(&Mcp::sendInitialClientComs, this, std::placeholders::_1)
    );
}

/**
 * Callback - post initial HELP message to the Grid after we have added process_search.task
 */
void Mcp::sendInitialClientComs(rclcpp::Client<grid_interfaces::srv::AddProcess>::SharedFuture future)
{
    // Get process ID from Grid's add_process service future, construct initial HELP message and publish
    int32_t processId = future.get()->process_id;
    RCLCPP_INFO(this->get_logger(), "Grid has sent back process ID %d for process_search.task. Publishing HELP message to the Grid.", processId);

    auto initialContact = std_msgs::msg::String();
    initialContact.data = m_mcpSubsystem.initialMessage(processId);
    m_gridComsPub->publish(initialContact);
    RCLCPP_INFO(this->get_logger(), "Published HELP message to the Grid.");
}
