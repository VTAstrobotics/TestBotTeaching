#include <stdio.h>

#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <sensor_msgs/msg/joy.h>
#include <std_msgs/msg/float32.h>

#include <rmw_microros/rmw_microros.h>

#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico_uart_transport.h"

const uint LED_PIN = 25;
const uint TEST_PIN = 12;
const uint LEFT_MOTOR = 10;
const uint RIGHT_MOTOR = 14;

rcl_publisher_t publisher;
std_msgs__msg__Float32 msg, left, right; // TODO: make these messages actually get data

void configurePWM() {

  // Tell GPIO 0 and 1 they are allocated to the PWM
  gpio_set_function(LEFT_MOTOR, GPIO_FUNC_PWM);
  gpio_set_function(RIGHT_MOTOR, GPIO_FUNC_PWM);

  // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
  uint slice_num = pwm_gpio_to_slice_num(10);
  uint slice_num2 = pwm_gpio_to_slice_num(14);

  // Set period of 4 cycles (0 to 3 inclusive)
  pwm_set_wrap(
      slice_num, 3000);
        pwm_set_wrap(
      slice_num2, 3000); // whatever percent out you want is just that number now

      pwm_set_clkdiv_mode(slice_num,PWM_DIV_FREE_RUNNING); // this should take the clock cycle from 125MHZ to 150hz for the spark maxes
      pwm_set_clkdiv_mode(slice_num2,PWM_DIV_FREE_RUNNING); 
      pwm_set_clkdiv(slice_num,255);
    pwm_set_clkdiv(slice_num2,255);

    //https://www.i-programmer.info/programming/148-hardware/14849-the-pico-in-c-basic-pwm.html?start=2


  pwm_set_chan_level(slice_num, PWM_CHAN_A, 2000);
  pwm_set_chan_level(slice_num2, PWM_CHAN_A, 2000); // should be stopped
  // Set the PWM running
  pwm_set_enabled(slice_num, true);
  pwm_set_enabled(slice_num2, true);

  // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the
  // correct slice and channel for a given GPIO.
}

uint32_t pwm_set_freq_duty(uint slice_num,
       uint chan,uint32_t f, int d)
       //f is frequency in HZ and d is duty cycle where d is a percent eg 75% is 75
       //from https://www.i-programmer.info/programming/148-hardware/14849-the-pico-in-c-basic-pwm.html?start=2
{
 uint32_t clock = 125000000;
 uint32_t divider16 = clock / f / 4096 + 
                           (clock % (f * 4096) != 0);
 if (divider16 / 16 == 0)
 divider16 = 16;
 uint32_t wrap = clock * 16 / divider16 / f - 1;
 pwm_set_clkdiv_int_frac(slice_num, divider16/16,
                                     divider16 & 0xF);
 pwm_set_wrap(slice_num, wrap);
 pwm_set_chan_level(slice_num, chan, wrap * d / 100);
 return wrap;
}

void timer_callback(rcl_timer_t *timer, int64_t last_call_time) {
  rcl_ret_t ret = rcl_publish(&publisher, &msg, NULL);
}

void left_callback(const void *msgin) {
  gpio_put(LED_PIN, 0);
  gpio_put(TEST_PIN, 1);

  const std_msgs__msg__Float32 *msgS = (const std_msgs__msg__Float32 *)msgin;

  // Message type shall be casted to expected type from void pointer.
  // const sensor_msgs__msg__Joy *msgJ = (const sensor_msgs__msg__Joy *)msgin;

  int power = (msgS[0].data * 50) + 50; //tuned
  msg.data = power;
  pwm_set_freq_duty(pwm_gpio_to_slice_num(LEFT_MOTOR),0, 340, power);
  rcl_publish(&publisher, &msg, NULL);
}

void right_callback(const void *msgin) {
  gpio_put(LED_PIN, 0);
  gpio_put(TEST_PIN, 1);

  const std_msgs__msg__Float32 *msgS = (const std_msgs__msg__Float32 *)msgin;

  // Message type shall be casted to expected type from void pointer.
  int power = (msgS[0].data * 50) + 50;
  msg.data = power;
  pwm_set_freq_duty(pwm_gpio_to_slice_num(RIGHT_MOTOR),0, 340, power);

  rcl_publish(&publisher, &msg, NULL);
}

int main() {
  rmw_uros_set_custom_transport(
      true, NULL, pico_serial_transport_open, pico_serial_transport_close,
      pico_serial_transport_write, pico_serial_transport_read);

  // apparently this function checks that a message is inited
  // std_msgs__msg__Float32__init();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
    configurePWM();
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

  if (ret != RCL_RET_OK) {
    // Unreachable agent, exiting program.

    return ret;
  }



  rclc_support_init(&support, 0, NULL, &allocator);

  rclc_node_init_default(&node, "pico_node", "", &support);

  // rclc_subscription_init_default(&joy_Subscriber, &node,
  // ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32), "/speeds");//TODO: can
  // I just get the type or do I need the sequence type
 
  rclc_publisher_init_default(
      &publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
      "pico_publisher");

  rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(200), timer_callback);

  rclc_executor_init(&executor, &support.context, 3, &allocator);

 


  rclc_executor_spin(&executor);
  while (true) {
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  }
  return 0;
}
