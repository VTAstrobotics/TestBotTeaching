import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32
import serial

class MinimalSubscriber(Node):

    def __init__(self):
        super().__init__('pythonDriving')
        print("node made")
        self.subscription = self.create_subscription(
            Joy,
            'joy',
            self.listener_callback,
            10)

        self.leftPub = self.create_publisher(
            Float32,
            'left',
            10
        )

        self.rightPub = self.create_publisher(
            Float32,
            'right',
            10
        )


    def listener_callback(self, Joymsg : Joy):
        self.leftPub.publish(Joymsg.axes[1])
        self.rightPub.publish(Joymsg.axes[1])
         



def main(args=None):
    rclpy.init(args=args)

    minimal_subscriber = MinimalSubscriber()

    rclpy.spin(minimal_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()