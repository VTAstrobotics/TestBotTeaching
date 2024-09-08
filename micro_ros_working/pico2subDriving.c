#include <stdio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/float32.h>
#include <sensor_msgs/msg/joy.h>

#include <rmw_microros/rmw_microros.h>

#include "pico/stdlib.h"
#include "pico_uart_transport.h"

const uint LED_PIN = 25;
const uint TEST_PIN = 12;
const uint LEFT_MOTOR = 10;
const uint RIGHT_MOTOR = 14;


rcl_publisher_t publisher;
std_msgs__msg__Float32 msg, left, right;




void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    rcl_ret_t ret = rcl_publish(&publisher, &msg, NULL);
}

void left_callback(const void *msgin)
{
    gpio_put(LED_PIN, 0);
    gpio_put(TEST_PIN, 1);
    
    const std_msgs__msg__Float32* msgS = (const std_msgs__msg__Float32*)msgin;
    
    // Message type shall be casted to expected type from void pointer.
    // const sensor_msgs__msg__Joy *msgJ = (const sensor_msgs__msg__Joy *)msgin;

    

    msg.data = msgS[0].data;
    rcl_publish(&publisher,&msg,NULL );
}

void right_callback(const void *msgin)
{
    gpio_put(LED_PIN, 0);
    gpio_put(TEST_PIN, 1);
    
    const std_msgs__msg__Float32* msgS = (const std_msgs__msg__Float32*)msgin;
    
    // Message type shall be casted to expected type from void pointer.
     const std_msgs__msg__Float32* msgJ = (const sensor_msgs__msg__Joy *)msgin;

    

    msg.data = msgS[0].data;
    rcl_publish(&publisher,&msg,NULL );
}

int main()
{
    rmw_uros_set_custom_transport(
        true,
        NULL,
        pico_serial_transport_open,
        pico_serial_transport_close,
        pico_serial_transport_write,
        pico_serial_transport_read);
    sensor_msgs__msg__Joy *joystickMSG = sensor_msgs__msg__Joy__create();
    left.data = -492921;
    right.data = -321821;
    // apparently this function checks that a message is inited std_msgs__msg__Float32__init();
    

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    


    rcl_timer_t timer;
    rcl_node_t node;
    rcl_allocator_t allocator;
    rclc_support_t support;
    rclc_executor_t executor;

    allocator = rcl_get_default_allocator();

    // Wait for agent successful ping for 2 minutes.
    const int timeout_ms = 1000;
    const uint8_t attempts = 120;

    rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, attempts);

    if (ret != RCL_RET_OK)
    {
        // Unreachable agent, exiting program.

        return ret;
    }

    // rcl_subscription_t joy_Subscriber;

    rcl_subscription_t left_Subscriber;
    rcl_subscription_t right_Subscriber;


    rclc_support_init(&support, 0, NULL, &allocator);

    rclc_node_init_default(&node, "pico_node", "", &support);

    // rclc_subscription_init_default(&joy_Subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32), "/speeds");//TODO: can I just get the type or do I need the sequence type

    rclc_subscription_init_default(&left_Subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32), "/left");//TODO: can I just get the type or do I need the sequence type
    rclc_subscription_init_default(&right_Subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32), "/right");//TODO: can I just get the type or do I need the sequence type


    rclc_publisher_init_default(
        &publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        "pico_publisher");

    rclc_timer_init_default(
        &timer,
        &support,
        RCL_MS_TO_NS(200),
        timer_callback);

    rclc_executor_init(&executor, &support.context, 3, &allocator);

    // rclc_executor_add_timer(&executor, &timer);

    rclc_executor_add_subscription(&executor, &left_Subscriber, &left, &left_callback, ON_NEW_DATA);
    rclc_executor_add_subscription(&executor, &right_Subscriber, &right, &right_callback, ON_NEW_DATA);

    rclc_executor_spin(&executor);
    while (true)
    {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    }
    return 0;
}
