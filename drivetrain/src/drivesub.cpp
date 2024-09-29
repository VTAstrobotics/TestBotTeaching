#include <functional>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/float32.hpp>

using std::placeholders::_1;

class DriveSub : public rclcpp::Node
{
public:
  DriveSub() : Node("drivetrainNode")
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 10, std::bind(&DriveSub::joy_callback, this, _1));
    left_speed_publisher_ = this->create_publisher<std_msgs::msg::Float32>("/left", 10);
    right_speed_publisher_ = this->create_publisher<std_msgs::msg::Float32>("/right", 10);
  }

private:
  void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
  {
    float left_speed = msg->axes[1];  // Adjust based on your joystick setup
    float right_speed = msg->axes[4]; // Adjust based on your joystick setup

    // Create messages for the left and right speed
    auto left_speed_msg = std::make_shared<std_msgs::msg::Float32>();
    auto right_speed_msg = std::make_shared<std_msgs::msg::Float32>();

    left_speed_msg->data = left_speed;
    right_speed_msg->data = right_speed;

    // Publish the speeds to the respective topics
    left_speed_publisher_->publish(*left_speed_msg);
    right_speed_publisher_->publish(*right_speed_msg);
  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr left_speed_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr right_speed_publisher_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DriveSub>());
  rclcpp::shutdown();
  return 0;
}
