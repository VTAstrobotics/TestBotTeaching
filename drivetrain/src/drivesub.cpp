#include <functional>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/string.hpp>
#include "uart.h"

using std::placeholders::_1;
class DriveSub : public rclcpp::Node
{  
Uart testBotCommunications{};
public:
  DriveSub() : Node("drivetrainNode")
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 10, std::bind(&DriveSub::joy_callback, this, _1));
  }

  ~DriveSub(){
      testBotCommunications.closeUart();
  }

private:
  void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
  {
    printf("test");
    RCLCPP_INFO(this->get_logger(), "Axes: [%f, %f]", msg->axes[0], msg->axes[1]);
    RCLCPP_INFO(this->get_logger(), "Buttons: [%d, %d]", msg->buttons[0], msg->buttons[1]);
    float leftSpeed = 90 * msg-> axes[0];
    float rightSpeed = 90 * msg-> axes[1];
    std::string MessageToArduino = (std::to_string(leftSpeed) + "," + std::to_string(rightSpeed)).c_str();
 
    testBotCommunications.sendUart(reinterpret_cast<unsigned char*>(const_cast<char*>(MessageToArduino.c_str())));
    
  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;

};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DriveSub>());
  rclcpp::shutdown();
  return 0;
}
