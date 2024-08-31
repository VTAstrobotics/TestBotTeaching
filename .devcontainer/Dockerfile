FROM ros:humble-ros-base-jammy

RUN apt-get update && apt-get install -y --no-install-recommends \
    ros-humble-desktop=0.10.0-1* \
    && rm -rf /var/lib/apt/lists/*

# Add nonroot user with same UID and GID as your host system
# (copied from https://code.visualstudio.com/remote/advancedcontainers/add-nonroot-user#_creating-a-nonroot-user)
ARG USERNAME=astro-dev
ARG USER_UID=1000
ARG USER_GID=$USER_UID
RUN groupadd --gid $USER_GID $USERNAME 

RUN useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME 
RUN sudo usermod -aG sudo ${USERNAME}
RUN apt-get update 
RUN apt-get install -y sudo 
RUN echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME 
RUN chmod 0440 /etc/sudoers.d/$USERNAME

# Switch from root to user
USER $USERNAME

# Update all packages
RUN sudo apt update && sudo apt upgrade -y

# Install Git
RUN sudo apt install -y git
RUN git config --global --add safe.directory /TestBotTeaching

# Rosdep update
RUN rosdep update

# Source the ROS setup file
RUN echo "source /opt/ros/${ROS_DISTRO}/setup.bash" >> ~/.bashrc

# Install clang-format for auto cpp formatting
RUN sudo apt-get install -y clang-format

# allow UART
RUN sudo chmod 777 /dev/ttyACM0
