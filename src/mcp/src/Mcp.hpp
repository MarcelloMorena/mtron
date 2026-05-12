#pragma once

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
//#include "grid_interfaces/srv/set_process_id.hpp"

#include <memory>
//#include <cstdint>

class Mcp : public rclcpp::Node
{
    public:
        Mcp();
    private:
        void initialise();
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr m_gridComsPub;

        // void receive_user_coms(const std_msgs::msg::String &msg);
        // void respond_user_coms();
        // void setProcessId(const std::shared_ptr<grid_interfaces::srv::SetProcessId::Request> request,
        //                     std::shared_ptr<grid_interfaces::srv::SetProcessId::Response> response);

        // rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_user_coms_sub;
        // std::unordered_map<int32_t, std::string> m_processIds;
        // rclcpp::TimerBase::SharedPtr m_initial_timer;

        // rclcpp::Service<grid_interfaces::srv::SetProcessId>::SharedPtr m_set_process_server;
};