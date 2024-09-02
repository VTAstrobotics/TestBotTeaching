import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from sensor_msgs.msg import Joy
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
        self.subscription  # prevent unused variable warning
        self.reader = serial.Serial(port = "/dev/ttyAMA0", baudrate = 115200, bytesize= serial.EIGHTBITS, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE)

    def get(self):
        data = None
        if self.reader.inWaiting() > 0:
            data = self.reader.read()
        try:
            data = data.decode()
        except Exception as error:
            data = ""
        finally:
            return data

    def send(self,msg):
        self.reader.write(msg)

    def getAllFeedback(self):
        res = ""
        feedback = self.get()
        while not feedback: feedback = self.get()
        while feedback:
            res += feedback
            feedback = self.get()

        return res



    def listener_callback(self, msg:Joy):
        self.get_logger().info('I heard: "%s"' % msg)
        self.send((str(msg.axes[1]) +  "," + str(msg.axes[4])).encode())
        print(self.getAllFeedback())
        


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