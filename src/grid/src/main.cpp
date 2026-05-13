#include "Grid.hpp"

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Grid>());
    rclcpp::shutdown();
}
