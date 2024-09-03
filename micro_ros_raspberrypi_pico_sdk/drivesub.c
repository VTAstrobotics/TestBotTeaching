// C standard includes.
#include <stdio.h>
#include <unistd.h>

// micro-ROS general includes with general functionality.
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

//Include publisher message type.
#include <std_msgs/msg/string.h>

// Include subscriber message type.
#include <sensor_msgs/msg/joy.h>


#include <rmw_microros/rmw_microros.h>
#include "pico/stdlib.h"
#include "pico_uart_transport.h"

// Macros to handle micro-ROS return codes, error handling should be customized for the target system.
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){ printf("Failed status on line %d: %d. Aborting.\n", __LINE__, (int) temp_rc); return 1; }}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){ printf("Failed status on line %d: %d. Continuing.\n", __LINE__, (int) temp_rc); }}

#define STRING_LEN 200

// rcl_publisher_t publisher;
std_msgs__msg__String msg;
int counter = 0;
const uint LED_PIN = 25;


void joy_callback(const void* msgin)
{
                              gpio_put(LED_PIN, 1);

  const sensor_msgs__msg__Joy* msg2 = (const sensor_msgs__msg__Joy*)msgin;
  float leftSpeed  = msg2->axes.data[1];
  float rightSpeed = msg2->axes.data[4];

  // std::string MessageToArduino = (std::u32string(leftSpeed) + "," + std::u32string(rightSpeed)).c_str();
  // RCLCPP_INFO(this->get_logger(), "Msg: [%s]", MessageToArduino.c_str());
  char str[256];
  snprintf(str, 255, "L: [%f], R: [%f]", leftSpeed, rightSpeed);
  // rcl_publish(&publisher, str, NULL);

}

int main()
{

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    rmw_uros_set_custom_transport(
      true,
      NULL,
      pico_serial_transport_open,
      pico_serial_transport_close,
      pico_serial_transport_write,
      pico_serial_transport_read
    );


    // Get configured allocator.
    rcl_allocator_t allocator = rcl_get_default_allocator();

    // Initialize support object.

    rclc_support_t support;
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

    // Node name and namespace.
    const char * node_name = "astronode";
    const char * node_namespace = "";

    // Create node.
    rcl_node_t node;
    rclc_node_init_default(&node, node_name, node_namespace, &support);

    // Topic name and type support.
    //const char * pub_topic_name = "astrotopic";
    //const rosidl_message_type_support_t * pub_type_support =
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);

    // Create publisher.
    // RCCHECK(rclc_publisher_init_default(
    //     &publisher,
    //     &node,
    //     pub_type_support,
    //     pub_topic_name));

    // Initialize publisher message memory.
    // char string_memory[STRING_LEN];
    // msg.data.data = &string_memory[0];
    // msg.data.size = 0;
    // msg.data.capacity = STRING_LEN;

    // Topic name and message type support.
    // rosidl_runtime_c__float__Sequence axes;

    
    const char * topic_name = "/joy";
    const rosidl_message_type_support_t * type_support =
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Joy);

    // Create subscriber.
    rcl_subscription_t subscriber;
    RCCHECK(rclc_subscription_init_default(
        &subscriber,
        &node,
        type_support,
        topic_name));

    // Initialize executor.
    rclc_executor_t executor;
    const size_t number_of_handles = 1;
    RCCHECK(rclc_executor_init(&executor, &support.context, number_of_handles, &allocator));

    // Add subscriber callback.
    sensor_msgs__msg__Joy Joystickmsg;
    RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &Joystickmsg, &joy_callback, ON_NEW_DATA));
      
    // Initialize subscriber message memory.
    // char string_memory[STRING_LEN];
    // msg.data.data = &string_memory[0];
    // msg.data.size = 0;
    // msg.data.capacity = STRING_LEN;

    // Spin forever

    RCSOFTCHECK(rclc_executor_spin(&executor));

    // Free used resources on spin error and exit.
    // This methods will free used memory even if connection with the Agent is lost.
    RCSOFTCHECK(rclc_executor_fini(&executor));
    //RCSOFTCHECK(rcl_publisher_fini(&publisher, &node));
    RCSOFTCHECK(rcl_subscription_fini(&subscriber, &node));
    RCSOFTCHECK(rcl_node_fini(&node));
    RCSOFTCHECK(rclc_support_fini(&support));

    return 1;
}
