# Container

## Clone Openzen

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
rm -rf build && cmake -S . -B build \
  -DCMAKE_PREFIX_PATH=/workspace/openzen/build \
  && cmake --build build -j$(nproc)
./build/example-cpp17 --verbose 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # --verbose
./build/example-cpp17 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # just dots per sample
```

* clean
```bash
cmake --build build --target clean
rm -rf build
```


## openzen v1.3.4
* Clone openzen
```bash
mkdir -p $HOME/repositories && cd $HOME/repositories && sudo rm -rf openzen
git clone https://bitbucket.org/lpresearch/openzen.git
cd $HOME/repositories/openzen
git checkout release/1.3.4
#BUILD
docker build -t openzen-buildbox -f docker/Dockerfile .
#RUN
docker run -it --rm \
  --net=host \
  --privileged \
  -v "$HOME/repositories/openzen:/root/openzen" \
  -v "$HOME/repositories/sentient:/root/sentient" \
  openzen-buildbox:latest

#BUILD OPENZEN
cd /root/openzen
#Remove build output folder (Conan + CMake artifacts)
rm -rf build/
# Optional: clear Conan local cache for this project's deps
conan remove "*" -c 
# install create relse and build
conan install . --output-folder=build --build=missing
cmake --preset conan-release
cmake --build --preset conan-release
cd /root/openzen/build/build/Release
sed -i 's|OpenZenTargets\.cmake|OpenZenConfigTargets.cmake|' OpenZenConfig.cmake

#BUILD SENTIENT EXAMPLE
cd /root/sentient/dependencies/openzen/cpp_examples
rm -rf build && cmake -S . -B build \
  -DCMAKE_PREFIX_PATH=/root/openzen/build/build/Release \
  && cmake --build build -j$(nproc)
./build/example-cpp17 --verbose 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # --verbose
./build/example-cpp17 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # just dots per sample

#IMU UTILS
cd /root/sentient/dependencies/openzen/imu_utils/
# clean and build
rm -rf build && cmake -S . -B build -DCMAKE_PREFIX_PATH=/root/openzen/build/build/Release \
  && cmake --build build -j$(nproc)
#run example  
./build/imu_utils17 --verbose 00:04:3E:53:ED:5B 100 921600
./build/imu_utils17 00:04:3E:53:ED:5B 100 921600

# TESTS > # need to add libgtest-dev into openzen/Dockerfile
./build/test_imu_utils
```


## Docker commands
```bash
docker images
docker ps
docker attach <ID> e.g. `docker attach $(docker ps -aq)`
docker stop <ID> e.g. `docker stop $(docker ps -aq)`
docker rename keen_einstein mycontainer
docker rmi --force <ID>

docker stop $(docker ps -a -q)
docker system prune -f --volumes #clean unused systems
