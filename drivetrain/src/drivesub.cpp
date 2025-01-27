#include <functional>
#include <memory>

//this include allows for the use of the ROS 2 system
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
//this include allows for the built-in message type
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/float32.hpp>
using std::placeholders::_1;
class DriveSub : public rclcpp::Node
{  
public:

  DriveSub() : Node("drivetrainNode")
  { // we've made the subscription but you need to make the two Float32 publishers you will need
    //creates subscription
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 10, std::bind(&DriveSub::joy_callback, this, _1));
      //declare your two publishers here
      publisher_left = this->create_publisher<std_msgs::msg::Float32>("left", 10);
      publisher_right = this->create_publisher<std_msgs::msg::Float32>("right", 10);
      // name_of_publisher = this->create_publisher<std_msgs::msgs::type>("name_of_topic", queue_num);

      /**publisher nodes actively send data called "messages" to a designated communication
       channel known as a "topic", making that data available for other nodes (called subscribers)
       to access and consume. 
       aka, node that produces and distributes information across the network. 
       creates and publishes new blocks 
      */

     /**
      * subscriber node is a computational unit that actively listens for and recieves 
      * messages published on a specific topic by other nodes. 
      * Acts as the receiver of data within te network, counter part of the publisher node!
      * Recieves what the publisher node sends out essentially. 
      */
  }

private:

  void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
  {
    /**
     * here you need to take in the msg and seperate it into a left and right speed variable and publish those values to the /left and /right topic
     */

    /**
     * const is constant, sensor_msgs::msg::Joy specifies the message type (specificially the joystick data e.g, axes and button states),
     * ::SharedPtr (alias to std::shared_ptr to a sensor_msgs::msg::Joy message. Smart pointer
     * that manages the lifetime of the object it points to, when destroyed object is too). 
     */

    //auto keyword type takes the type of what is on the other side of the =. (auto does it...)

    //created left and right speed variables that use the message (msg) to grab data
    // look at the x-axe = axes[0] 
    // range for speed is -1 - 1
    // 

    //left stick, data
    auto left_speed = std_msgs::msg::Float32();
    left_speed.data = msg->axes[0];

    //right stick, data
    auto right_speed = std_msgs::msg::Float32();
    right_speed.data = msg->axes[4]; 
   

    //logging to see if message was sent
    RCLCPP_INFO(this->get_logger(), "Publishing left speed: %f\n", left_speed.data);
    RCLCPP_INFO(this->get_logger(), "Publishing right speed: %f\n", right_speed.data);

   //publishing message to the respective publishers

   publisher_left->publish(left_speed);
   publisher_right->publish(right_speed);

  }
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_left;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_right;
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;

};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DriveSub>());
  rclcpp::shutdown();
  // this is the code that actually makes the node run
  return 0;
}


