# Container

## Clone Openzen

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

#TWOIMUS
cd /root/sentient/dependencies/openzen/twoimus
rm -rf build && cmake -S . -B build -DCMAKE_PREFIX_PATH=/root/openzen/build/build/Release \
  && cmake --build build -j$(nproc)
#run
./build/twoimus17 00:04:3E:6F:37:7E 00:04:3E:53:ED:5B 100 921600
./build/twoimus17 --verbose 00:04:3E:6F:37:7E 00:04:3E:53:ED:5B 100 921600

#THREE_IMUS
cd /root/sentient/dependencies/openzen/threeimus
rm -rf build && cmake -S . -B build -DCMAKE_PREFIX_PATH=/root/openzen/build/build/Release \
  && cmake --build build -j$(nproc)
#run
./build/threeimus17 00:04:3E:6F:37:7E 00:04:3E:53:ED:5B 00:04:3E:6F:37:95 100 921600
./build/threeimus17 00:04:3E:6F:37:7E 00:04:3E:53:ED:5B 00:04:3E:6F:37:95 100 921600 --verbose
```


## Docker commands
```bash
docker images
docker ps
docker attach <ID> e.g. `docker attach $(docker ps -aq)`
docker stop <ID> e.g. `docker stop $(docker ps -aq)`
docker rename keen_einstein mycontainer
docker rmi --force <ID>
docker exec -it <ID> bash #ID $(docker ps -aq)
docker exec -it -u root <ID> bash #ID $(docker ps -aq)

docker stop $(docker ps -a -q)
docker system prune -f --volumes #clean unused systems
