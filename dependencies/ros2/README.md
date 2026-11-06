# OpenZen Node for ROS 2

This software allows to forward sensor data from [OpenZen](https://bitbucket.org/lpresearch/openzen/) 
sensors connected via OpenZen to ROS version 2.

## Requirements

### Launch container
Probably the first step is to build container. See [ros2README.md](ros2README.md).
Otherwise you can launch container

```bash
cd $HOME/repositories/sentient/dependencies/ros2
IMAGE=ubunturos2noblejazzy VER=v0.0.3
#?xhost +local:docker

docker run -it --rm \
  --net=host \
  --privileged \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  -v /run/dbus:/run/dbus:ro \
  -v /dev:/dev \
  -v "$HOME/repositories/sentient:/home/rosuser/sentient" \
  -v "$HOME/repositories/sentient/dependencies/ros2:/home/rosuser/ros2_ws/src/openzenros2" \
  -v "$HOME/repositories/openzen:/home/rosuser/ros2_ws/src/openzenros2/openzen" \
  -v "$HOME/repositories/sentient/dependencies/VINS-MONO-ROS2:/home/rosuser/ros2_ws/src/VINS-MONO-ROS2" \
  -v "$HOME/datasets/sentient/bags:/home/rosuser/bags" \
  ${IMAGE}:${VER}

#todo bind /home/rosuser/ros2_ws/vins_mono_ws/output
```

* Once in the container, you can check devices
```bash
v4l2-ctl --list-devices
```

## Compile ros2 package

To compile this driver in your ROS2 setup using openzen v1.3.4
```bash
#
# Compile openzen
#
cd /home/rosuser/sentient/dependencies/ros2
bash compile-openzenros2-package.bash
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
cd ceres-solver-2.1.0 && mkdir build && cd build
cmake -DEXPORT_BUILD_DIR=ON -DMINIGLOG=ON -DEigen3_DIR=/usr/local/share/eigen3/cmake .. \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        ../
make -j $(nproc) && sudo make install -j $(nproc)
# 
# VINS-fusion-ROS2:ros2_jazzy (already as submodule)
#
mkdir -p $HOME/ros2_ws/vins_mono_ws/output
#
# expected-lite-0.10.0
cd /home/rosuser/
wget -O expected-lite-0.10.0.tar.gz https://github.com/martinmoene/expected-lite/archive/refs/tags/v0.10.0.tar.gz
tar -xzf expected-lite-0.10.0.tar.gz && cd expected-lite-0.10.0 && mkdir build && cd build
cmake .. -DEXPECTED_LITE_OPT_BUILD_TESTS=OFF -DEXPECTED_LITE_OPT_BUILD_EXAMPLES=OFF
make -j $(nproc) && sudo make install -j $(nproc)
#
# build ros2 packages
cd /home/rosuser/ros2_ws && rm -rf build log install
colcon build --cmake-force-configure \
  --cmake-args "-DCMAKE_BUILD_TYPE=Release" \
  "-DCMAKE_POLICY_VERSION_MINIMUM=3.5" \
  "-Dexpected-lite_DIR=/usr/local/lib/cmake/expected-lite" \
  "-DCMAKE_TOOLCHAIN_FILE=/home/rosuser/ros2_ws/src/openzenros2/openzen/build/build/Release/generators/conan_toolchain.cmake"
#
# source installation
source ~/ros2_ws/install/setup.bash
```

## Launch and run ros2 to stream data from sensors
Swith on sensor 

### Terminal 1

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

### Terminal 2
Open a new terminal to run same image
```bash
docker exec -it $(docker ps -aq) bash 
source ~/ros2_ws/install/setup.bash
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
# Terminal 2
# If you want to keep using map as your Fixed Frame 
# (for example, to later combine the camera with other robot data), 
# you need to provide the missing transform. 
ros2 run tf2_ros static_transform_publisher 0 0 0 0 0 0 map usb_cam

# rviz2 #In RViz2, click Add -> select Image -> set Image Topic to /camera/image_raw. 
```

Plot some values (for example linear acceleration) with 

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

## VOI

* Downloaded already locally `ar_box.bag` and convert from ros1 to ros2 format
```bash
cd /home/rosuser/ && mkdir -p bags && cd bags
#
# conver files
rosbags-convert --src /home/rosuser/bags/ar_box.bag --dst /home/rosuser/bags/ar_box
```


* ar_demo 3dm_bag
```bash
#Terminal 1
ros2 launch ar_demo 3dm_bag.launch.py # for featuer tracking, backend optimization, ar demo and rviz2.
#Terminal 2
ros2 bag play /home/rosuser/bags/ar_box
```

* Visual-inertial odometry and loop closure
```bash
#Terminal 1
ros2 launch feature_tracker vins_feature_tracker.launch.py      
#Terminal 2
# for feature tracking and rviz2
ros2 launch vins_estimator euroc.launch.py # for backend optimization and loop closure
#Terminal 3
ros2 bag play /home/rosuser/bags/MH_01_easy # for ros2 bag
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
