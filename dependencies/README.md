# Dependencies


## add VINS-MONO-ROS2.git as submodule (just do it once)
```bash
git submodule add -b ros2_jazzy https://github.com/mxochicale/VINS-MONO-ROS2.git dependencies/VINS-MONO-ROS2
# Sync and Pull Latest from Remote Branch
git submodule update --remote dependencies/VINS-MONO-ROS2
#IF NEEDED remove index
# git rm --cached dependencies/VINS-MONO-ROS2
# rm -rf .git/modules/dependencies/VINS-MONO-ROS2
```

## data 
```bash
mkdir -p $HOME/datasets/sentient/bags && cd $HOME/datasets/sentient/bags
wget "https://www.dropbox.com/scl/fi/q18lot4bfs1fqrctclz7b/ar_box.bag?rlkey=16yrxnwnt2fcutwwzwhlevd1n&e=2&dl=1" -O ar_box.bag
```

## OS
```
$ hostnamectl

 Static hostname: --
       Icon name: computer-laptop
         Chassis: laptop
      Machine ID: --
         Boot ID: --
Operating System: Ubuntu 22.04.1 LTS              
          Kernel: Linux 5.15.0-56-generic
    Architecture: x86-64
 Hardware Vendor: --

```

## GPU
```
$ nvidia-smi -q

==============NVSMI LOG==============

Timestamp                                 : Sat Dec 17 13:27:52 2022
Driver Version                            : 520.61.05
CUDA Version                              : 11.8

Attached GPUs                             : 1
GPU 00000000:01:00.0
    Product Name                          : NVIDIA RTX A2000 8GB Laptop GPU
    Product Brand                         : NVIDIA RTX
    Product Architecture                  : Ampere

```
