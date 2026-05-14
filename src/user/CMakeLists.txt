cmake_minimum_required(VERSION 3.8)
project(user)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(grid_interfaces REQUIRED)

add_executable(user src/main.cpp src/User.cpp src/UserSubsystem.cpp)
ament_target_dependencies(user rclcpp rclcpp_action std_msgs grid_interfaces)

target_include_directories(user PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(user PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(TARGETS user
  DESTINATION lib/${PROJECT_NAME})

ament_package()
