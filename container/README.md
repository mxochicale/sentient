# Container

## Clone Openzen

Clone openzen 
```
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
rm -rf build && cmake -S . -B build && cmake --build build -j$(nproc)
./build/example-cpp17 --verbose 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # --verbose
./build/example-cpp17 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # just dots per sample
```

* clean
```bash
cmake --build build --target clean
rm -rf build
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
