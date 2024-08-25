#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"

using std::placeholders::_1;

class DriveSub : public rclcpp::Node
{
public:
  DriveSub() : Node("drivesub")
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 10, std::bind(&DriveSub::joy_callback, this, _1));
  }

private:
  void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg) const
  {
    RCLCPP_INFO(this->get_logger(), "Axes: [%f, %f]", msg->axes[0], msg->axes[1]);
    RCLCPP_INFO(this->get_logger(), "Buttons: [%d, %d]", msg->buttons[0], msg->buttons[1]);
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DriveSub>());
  rclcpp::shutdown();
  return 0;
}
