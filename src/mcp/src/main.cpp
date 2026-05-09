#include "Mcp.hpp"

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Mcp>());
    rclcpp::shutdown();
}