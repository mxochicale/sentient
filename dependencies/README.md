# Setting up conda env 

## 1. Mamba
Install [mamba](https://github.com/mxochicale/code/tree/main/mamba) and create [...VE](ve.yml).

## 2. Installing dependencies

* Python package versions (log for machine with CUDA GPU)
```
mamba activate rtt4ssaVE
python packages_versions.py 


2023-08-09 07:26:39.823757: I tensorflow/core/util/port.cc:110] oneDNN custom operations are on. You may see slightly different numerical results due to floating-point round-off errors from different computation orders. To turn them off, set the environment variable `TF_ENABLE_ONEDNN_OPTS=0`.
2023-08-09 07:26:39.891305: I tensorflow/core/platform/cpu_feature_guard.cc:182] This TensorFlow binary is optimized to use available CPU instructions in performance-critical operations.
To enable the following instructions: SSE4.1 SSE4.2 AVX AVX2 AVX_VNNI FMA, in other operations, rebuild TensorFlow with the appropriate compiler flags.
python: 3.10.12 | packaged by conda-forge | (main, Jun 23 2023, 22:40:32) [GCC 12.3.0]
opencv: 4.8.0
torch: 2.0.0.post200
torch cuda_is_available: True
torch cuda version: 11.2
torch cuda.device_count  1
PIL version: 10.0.0
Tensorflow version: 2.12.1


```

* OS
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

* GPU
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
