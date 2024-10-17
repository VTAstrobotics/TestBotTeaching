#!/bin/bash

# build the thing
colcon build --symlink-install

# source it
. install/setup.bash

# allow UART
sudo chmod 777 /dev/ttyACM0
sudo chmod 777 /dev/input/event*
# this one may be unneeded
sudo chmod 777 /dev/input/js*

# LAST COMMAND
/bin/bash
