#include "controller/controller_node.hpp"
#include <algorithm>

ControllerNode::ControllerNode() : Node("controller_node")
{
  brake_command_subscription_ = this->create_subscription<std_msgs::msg::Bool>(
      "brake_command_topic", 10,
      std::bind(&ControllerNode::brake_command_callback, this, std::placeholders::_1));

  distance_subscription_ = this->create_subscription<std_msgs::msg::Float32>(
      "distance_topic", 10,
      std::bind(&ControllerNode::distance_callback, this, std::placeholders::_1));

  brake_distance_subscription_ = this->create_subscription<std_msgs::msg::Float32>(
      "brake_distance_topic", 10,
      std::bind(&ControllerNode::brake_distance_callback, this, std::placeholders::_1));

  brake_output_publisher_ = this->create_publisher<std_msgs::msg::Float32>("brake_output_topic", 10);
}

void ControllerNode::brake_command_callback(const std_msgs::msg::Bool & msg)
{
  apply_brake(msg.data);
}

void ControllerNode::distance_callback(const std_msgs::msg::Float32 & msg)
{
  current_distance_ = msg.data;
}

void ControllerNode::brake_distance_callback(const std_msgs::msg::Float32 & msg)
{
  brake_distance_ = msg.data;
}

void ControllerNode::apply_brake(bool brake_active)
{
  is_braking_ = brake_active;

  auto brake_output = std_msgs::msg::Float32();

  if (brake_active && brake_distance_ > 0.0)
  {
    double error = brake_distance_ - current_distance_;

    if (error <= 0.0)
    {
      brake_output.data = 0.0;
    }
    else
    {
      double k = 1.0 / brake_distance_;
      brake_output.data = std::min(1.0, k * error);
    }
  }
  else
  {
    brake_output.data = 0.0;
  }

  brake_output_publisher_->publish(brake_output);
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ControllerNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}