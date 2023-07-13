# Setting up conda env 

## 1. Conda
Install [conda](https://github.com/mxochicale/code/tree/main/conda) and create [...VE](ve.yml).

## 2. Installl dependencies

* Python package versions (log for machine with CUDA GPU)
```
$ mamba activate ai-surg-skills-VE
$ python package_versions.py 


python: 3.8.16 | packaged by conda-forge | (default, Feb  1 2023, 16:01:55) 
[GCC 11.3.0]
opencv: 4.7.0
torch: 2.0.0.post200
torch cuda_is_available: True
torch cuda version: 11.2
torch cuda.device_count  1
PIL version: 9.5.0

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
