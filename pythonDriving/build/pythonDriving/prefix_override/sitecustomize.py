import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/cameron/Astrobotics/wtfisgoingon/micro_ros_raspberrypi_pico_sdk/pythonDriving/install/pythonDriving'
