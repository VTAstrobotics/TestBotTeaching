#!/bin/bash

echo "changing dir"
cd /TestBotTeaching/



colcon build --symlink-install

echo "Sourcing"
source install/setup.bash

echo "launching robot control"

ros2 run pythonDriving pythonDriving&

echo "Starting Joystick control"
ros2 run joy joy_node