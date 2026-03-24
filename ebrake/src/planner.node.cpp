#include "ebrake/planner_node.hpp"

PlannerNode::PlannerNode() : Node("planner_node")
{
  
    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "input_topic", 10, std::bind(&PlannerNode::subscriber_callback, this, std::placeholders::_1)); // TODO: change it to the corrcet topic name 

        publisher_ = this->create_publisher<std_msgs::msg::String>("output_topic", 10); // TODO: change it to the correct topic name
        RCLCPP_INFO(this->get_logger(), "Planner node is running");
}


int main (int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PlannerNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}   