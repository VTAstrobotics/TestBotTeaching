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
    LeftPublisher_ = this->create_publisher<std_msgs::msg::Float32>("/leftJoy", 10);
    RightPublisher_ = this->create_publisher<std_msgs::msg::Float32>("/rightJoy", 10);
  }


private:
  void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
  {

    auto left = std_msgs::msg::Float32();
    auto right = std_msgs::msg::Float32();

    /**
     * here you need to take in the msg and seperate it into a leftJoy and rightJoy speed variable and publish those values to the /leftJoy and /rightJoy topic
     */
    float leftJoy  = msg-> axes[1];
    float rightJoy = msg-> axes[4];
    left.data = leftJoy;
    right.data = rightJoy;
    LeftPublisher_->publish(left);
    RightPublisher_->publish(right);

  }
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr LeftPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr RightPublisher_;

};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DriveSub>());
  rclcpp::shutdown();
  // this is the code that actually makes the node run
  return 0;
}
