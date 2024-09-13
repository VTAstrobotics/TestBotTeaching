#!/bin/bash

# build the thing
colcon build --symlink-install

# source it
. install/setup.bash

# allow UART
sudo chmod 777 /dev/ttyACM0

# LAST COMMAND
/bin/bash
