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
cd /home/rosuser/ros2_ws/src/openzenros2/openzen && rm -rf build/
conan profile detect
mkdir -p ~/.conan2/profiles
cp docker/conan_profile_linux ~/.conan2/profiles/linux
# clear Conan local cache for this project's deps
conan remove "*" -c 
# install create relse and build
conan install . --output-folder=build --build=missing
conan install --requires=expected-lite/0.9.0 --build=missing
cmake --preset conan-release
cmake --build --preset conan-release
cd /home/rosuser/ros2_ws/src/openzenros2/openzen/build/build/Release
sed -i 's|OpenZenTargets\.cmake|OpenZenConfigTargets.cmake|' OpenZenConfig.cmake
#build ros2 package
cd /home/rosuser/ros2_ws
rm -rf build log install
colcon build --cmake-force-configure \
  --cmake-args "-DCMAKE_BUILD_TYPE=Release" \
  "-DCMAKE_TOOLCHAIN_FILE=/home/rosuser/ros2_ws/src/openzenros2/openzen/build/build/Release/generators/conan_toolchain.cmake"
source install/setup.bash
```

* build ros2 package with openzen v1.3.1
```bash
cd /home/rosuser/ros2_ws
rm -rf build log install
colcon build --cmake-args -DCMAKE_POLICY_VERSION_MINIMUM=3.5
source install/setup.bash
```

## Launch and run ros2 to stream data from sensors

* Camera
```bash
ros2 launch openzen_driver usb_camera.launch.py
```

* OpenZen ROS2 driver
```bash
ros2 run openzen_driver openzen_node --ros-args --remap __ns:=/openzen
```

By default, it will connect to the first available sensor. If you want to connect to
a specific sensor and different baudrate, you can use the serial name and the baudrate of the sensor as parameters, for example:
```bash
ros2 run openzen_driver openzen_node --ros-args --remap __ns:=/openzen -p sensor_name:="LPMSB2-53ED5B" -p baudrate:=115200
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

