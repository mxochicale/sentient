
# Second steps build conianter with foxiy ros2

* build container with ros2
```bash
cd $HOME/repositories/sentient/ros2
IMAGE=ubunturos2focalfoxy VER=v0.0.1 #Versions
docker build -t "${IMAGE}:${VER}" -f DockerfileFocalFoxy .
#ros2 noble
IMAGE=ubunturos2noblejazzy VER=v0.0.1 #Versions
docker build -t "${IMAGE}:${VER}" -f DockerfileNobleJazzy .
```


NOTE.
Change the DDS Middleware (RMW Implementation)
The default ROS 2 middleware in Jazzy (rmw_fastrtps_cpp) has been reported to cause "choppy" visualization in rqt for high-bandwidth or high-frequency data streams like camera feeds or IMU topics. Users have found that switching to rmw_cyclonedds_cpp or rmw_zenoh_cpp makes the visualization "smooth" or "super smooth" .

You can test this by changing the environment variable before launching your node and rqt_plot:

sudo apt update
sudo apt install ros-jazzy-rmw-cyclonedds-cpp
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
ros2 run rqt_plot rqt_plot /openzen/data/linear_acceleration
                           

* Launch container
```bash
#?xhost +local:docker

docker run -it --rm \
  --net=host \
  --privileged \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  -v /run/dbus:/run/dbus:ro \
  -v "$HOME/repositories/sentient:/home/rosuser/sentient" \
  -v "$HOME/repositories/sentient/ros2:/home/rosuser/ros2_ws/src/openzenros2" \
  -v "$HOME/repositories/openzen:/home/rosuser/ros2_ws/src/openzenros2/openzen" \
  ${IMAGE}:${VER}


```




# First Stesp build container for openzen and chenck the bin file can stream data from snenoser via bluetoht




## Openzen v1.3.1
```bash
mkdir -p $HOME/repositories && cd $HOME/repositories && sudo rm -rf openzen
git clone --branch 1.3.1 --single-branch --recurse-submodules https://bitbucket.org/lpresearch/openzen.git
```

## Set up and build
```bash
cd $HOME/repositories/sentient
IMAGE=ubuntu VER=v0.0.1 #Versions
docker build -t "${IMAGE}:${VER}" -f Dockerfile .
```

* Run 
```bash
# passing BT hardware and D-Bus socket into the container
docker run -it --rm \
  --net=host \
  --privileged \
  -v /run/dbus:/run/dbus:ro \
  -v "$HOME/repositories/sentient":/workspace \
  -v "$HOME/repositories/openzen":/workspace/openzen \
  ${IMAGE}:${VER}

```

## Discover devices
```bash
bluetoothctl devices
bluetoothctl pair 00:04:3E:53:ED:5B
```

## Build openzen

```bash
cd /workspace/openzen && rm -rf build
cmake -S . -DZEN_BLUETOOTH=ON -B build
cmake --build build -j$(nproc)
cd /workspace/openzen/build
sed -i 's|OpenZenTargets\.cmake|OpenZenConfigTargets.cmake|' OpenZenConfig.cmake
cat OpenZenConfig.cmake
```

## Build and run openzen examples
* Build and run
```bash
cd /workspace/dependencies/openzen/cpp_examples
rm -rf build && cmake -S . -B build  -DCMAKE_PREFIX_PATH=/workspace/openzen/build \
  && cmake --build build -j$(nproc)
./build/example-cpp17 --verbose 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # --verbose
./build/example-cpp17 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # just dots per sample
```

* clean
```bash
cmake --build build --target clean
rm -rf build
```


