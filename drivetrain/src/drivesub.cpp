#include <functional>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/float32.hpp>


using std::placeholders::_1;
class DriveSub : public rclcpp::Node
{  
public:
  DriveSub() : Node("drivetrainNode")
  { // we've made the subscription but you need to make the two Float32 publishers you will need
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 10, std::bind(&DriveSub::joy_callback, this, _1));
      //declare your two publishers here
    pub1_ = this->create_publisher<std_msgs::msg::Float32>("/left", 10);
    pub2_= this->create_publisher<std_msgs::msg::Float32>("/right", 10);
  }


private:
  void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
  {
    /**
     * here you need to take in the msg and seperate it into a left and right speed variable and publish those values to the /left and /right topic
     */

    float pub1_speed = msg->axes[1];  // Left joystick
    float pub2_speed = msg->axes[4]; // Right joystick
    
    auto pub1_msg = std_msgs::msg::Float32();
    auto pub2_msg = std_msgs::msg::Float32();
    
    pub1_msg.data = pub1_speed;
    pub2_msg.data = pub2_speed;

    pub1_->publish(pub1_msg);
    pub2_->publish(pub2_msg);


  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub1_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub2_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DriveSub>());
  rclcpp::shutdown();
  // this is the code that actually makes the node run
  return 0;
}
