# OpenZen Node for ROS 2

This software allows to forward sensor data from [OpenZen](https://bitbucket.org/lpresearch/openzen/) sensors connected via OpenZen to ROS version 2.

## Requirements

### Build container
See [ros2README.md](../ros2/ros2README.md) to build container and further information.

```bash
cd $HOME/repositories/sentient/ros2
IMAGE=ubunturos2noblejazzy VER=v0.0.2 #Versions

xhost +local:docker

docker run -it --rm \
  --net=host \
  --privileged \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  -v /run/dbus:/run/dbus:ro \
  -v /dev:/dev \
  -v "$HOME/repositories/sentient:/home/rosuser/sentient" \
  -v "$HOME/repositories/sentient/ros2:/home/rosuser/ros2_ws/src/openzenros2" \
  -v "$HOME/repositories/openzen:/home/rosuser/ros2_ws/src/openzenros2/openzen" \
  ${IMAGE}:${VER}

```

* Check devices
```bash
v4l2-ctl --list-devices
```

## Compile ros2 package

To compile this driver in your ROS2 setup using openzen v1.3.4
```bash
#
# Compile openzen
#
cd /home/rosuser/sentient/dependencies
bash compile-ros2-package.bash
#
# Eigen v3.4.9
cd /home/rosuser/
wget -O eigen-3.4.1.tar.gz https://gitlab.com/libeigen/eigen/-/archive/3.4.1/eigen-3.4.1.tar.gz
tar zxf eigen-3.4.1.tar.gz
cd ~/eigen-3.4.1 && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && sudo make install -j $(nproc)
# 
# Ceres solver v2.1.0
cd /home/rosuser/
wget -O ceres-solver-2.1.0.tar.gz https://github.com/ceres-solver/ceres-solver/archive/refs/tags/2.1.0.tar.gz
tar zxf ceres-solver-2.1.0.tar.gz
cd ceres-solver-2.1.0
mkdir build && cd build
cmake -DEXPORT_BUILD_DIR=ON -DMINIGLOG=ON -DEigen3_DIR=/usr/local/share/eigen3/cmake .. \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        ../
make -j $(nproc) # number of cores
sudo make install -j $(nproc)
# 
# VINS-fusion-ROS2
cd /home/rosuser/ros2_ws/src
git clone https://github.com/dongbo19/VINS-MONO-ROS2.git
cd VINS-MONO-ROS2
git checkout ros2_jazzy
sudo apt update
sudo apt install -y libgoogle-glog-dev

#
#?Jazzy, the header file was changed from cv_bridge.h to cv_bridge.hpp 
#?cd ~/ros2_ws/src/VINS-Fusion-ROS2/loop_fusion
#?grep -rl "cv_bridge/cv_bridge.h" . | xargs sed -i 's|cv_bridge/cv_bridge.h|cv_bridge/cv_bridge.hpp|g'
#
# expected-lite
cd ~
wget -O expected-lite-0.10.0.tar.gz https://github.com/martinmoene/expected-lite/archive/refs/tags/v0.10.0.tar.gz
tar -xzf expected-lite-0.10.0.tar.gz
cd expected-lite-0.10.0 && mkdir build && cd build
cmake .. -DEXPECTED_LITE_OPT_BUILD_TESTS=OFF -DEXPECTED_LITE_OPT_BUILD_EXAMPLES=OFF
sudo make install -j $(nproc)
# build ros2 packages
cd /home/rosuser/ros2_ws && rm -rf build log install
colcon build --cmake-force-configure \
  --cmake-args "-DCMAKE_BUILD_TYPE=Release" \
  "-DCMAKE_POLICY_VERSION_MINIMUM=3.5" \
  "-Dexpected-lite_DIR=/usr/local/lib/cmake/expected-lite" \
  "-DCMAKE_TOOLCHAIN_FILE=/home/rosuser/ros2_ws/src/openzenros2/openzen/build/build/Release/generators/conan_toolchain.cmake"
#
source install/setup.bash
```

## Launch and run ros2 to stream data from sensors

* OpenZen ROS2 driver
```bash
ros2 run openzen_driver openzen_node --ros-args --remap __ns:=/openzen #sampling_rate:=50
ros2 run openzen_driver openzen_node --ros-args --remap __ns:=/openzen  -p sampling_rate:=100
```

By default, it will connect to the first available sensor. If you want to connect to
a specific sensor and different baudrate, you can use the serial name and the baudrate of the sensor as parameters, for example:
```bash
ros2 run openzen_driver openzen_node --ros-args --remap __ns:=/openzen -p sensor_name:="LPMSB2-53ED5B" -p baudrate:=115200
```

* Camera
```bash
ros2 launch openzen_driver usb_camera.launch.py
```

* ar_demo 3dm_bag
```bash
ros2 launch ar_demo 3dm_bag.launch.py # for featuer tracking, backend optimization, ar demo and rviz2.
ros2 bag play /home/rosuser/ros2_ws/src/openzenros2/ar_box/ar_box_ros2

```
* convert ros1 to ros2 format
```
cd 
wget "https://www.dropbox.com/scl/fi/q18lot4bfs1fqrctclz7b/ar_box.bag?rlkey=16yrxnwnt2fcutwwzwhlevd1n&e=2&dl=1" -O ar_box.bag
sudo apt install pipx
pipx ensurepath
pipx install rosbags
source  ~/.bashrc
rosbags-convert --src /home/rosuser/ros2_ws/src/openzenros2/ar_box/ar_box.bag --dst /home/rosuser/ros2_ws/src/openzenros2/ar_box/ar_box_ros2
```

Open a new terminal to run same image
```bash
docker exec -it $(docker ps -aq) bash 
```

Now you can see variables, topic type (`sensor_msgs/msg/Imu`), and print values:
```bash
ros2 topic list
ros2 topic info /openzen/data --verbose
ros2 topic echo /openzen/data
```

Camera `sensor_msgs/msg/CameraInfo`
```bash
ros2 topic info /openzen/camera/image_raw --verbose
ros2 run rqt_image_view rqt_image_view /openzen/camera/image_raw
rviz2 #In RViz2, click Add -> select Image -> set Image Topic to /camera/image_raw. 
```
Or plot some values (for example linear acceleration) with 

```bash
ros2 run rqt_plot rqt_plot /openzen/data/linear_acceleration
ros2 run rqt_plot rqt_plot /openzen/data/orientation
```

`openzen_lpms.launch` shows data acquisition and plotting using openzen_sensor_node:
```bash
ros2 launch openzen_driver openzen_lpms.launch.py
```

To change to autocalibration setting via the command line:
```bash
ros2 service call /enable_gyro_autocalibration std_srvs/SetBool "{data: true}"
#waiting for service to become available...
```

To trigger an gyroscope calibration:
```bash
ros2 service call /calibrate_gyroscope std_srvs/Trigger
#waiting for service to become available...
```

## Reference
https://bitbucket.org/lpresearch/openzenros2/src/master/

## build ros2 package with openzen v1.3.1
```bash
cd /home/rosuser/ros2_ws
rm -rf build log install
colcon build --cmake-args -DCMAKE_POLICY_VERSION_MINIMUM=3.5
source install/setup.bash
```
