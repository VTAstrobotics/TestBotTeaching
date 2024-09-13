#include <functional>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/string.hpp>

using std::placeholders::_1;
class DriveSub : public rclcpp::Node
{  
public:
  DriveSub() : Node("drivetrainNode")
  { // we've made the subscription but you need to make the two Float32 publishers you will need
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 10, std::bind(&DriveSub::joy_callback, this, _1));
  }


private:
  void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
  {
    /**
     * here you need to take in the msg and seperate it into a left and right speed variable and publish those values to the /left and /right topic
     */



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
