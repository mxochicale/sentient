# OpenZen under GNU/Linux Ubuntu

1. Check OS distribution, kernel and arquitecture
* OS: Ubuntu 22.04.1 LTS              
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

* OS: Ubuntu 20.04.1 LTS
```
$ hostnamectl

   Static hostname: ---
         Icon name: computer-laptop
           Chassis: laptop
        Machine ID: ---
           Boot ID: ---
  Operating System: Ubuntu 20.04.1 LTS
            Kernel: Linux 5.15.0-73-generic
```


* Python package versions
```
$ conda activate *VE
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


2. Install dependencies
```
sudo apt-get update
sudo apt-get install git cmake
sudo apt-get install libbluetooth-dev
sudo apt install build-essential -y #> sudo apt-get install gcc-7
sudo apt-get install python3 
sudo apt-get install python3-dev
 
#Install Qt (5.11.2 or higher) if you need Bluetooth Low Energy support: 
#sudo apt-get install qtbase5-dev qtconnectivity5-dev
``` 

3. Clone openzen 
```
mkdir -p $HOME/repositories && cd $HOME/repositories ## suggested path
git clone --recurse-submodules https://bitbucket.org/lpresearch/openzen.git
cd openzen 
```

    3.1 You might like to edit `CMakeLists.txt` but also do `cmake -DZEN_PYTHON=ON ..`
    ```
    vim CMakeLists.txt
    option(ZEN_PYTHON "Compile Python bindings for OpenZen" ON) #LINE72
    ```

4. build and make
```
conda activate *VE
cd $HOME/repositories/openzen && rm -rf build && mkdir build && cd build
cmake -DZEN_PYTHON=ON ..
make -j4
```

* To test: `-DPython3_EXECUTABLE:PATH=${PYTHON_EXECUTABLE}` [:link:](https://github.com/Slicer/Slicer/issues/5498)
 
5. Switching on IMU and test connection

```
$ hcitool dev
Devices:
	hci0	00:30:91:10:00:26

$ hcitool scan
Scanning ...
	00:04:3E:53:ED:5B	LPMSB2-53ED5B
```

5.1 Setting up LPSENSOR device 
	* Go to bluetooth settings
	* Select device LPMSB2-???
	* Confirm Bluetooth PIN 

6. Now you can run the OpenZenExample:
```
cd $HOME/repositories/openzen/examples
./OpenZenExample
```

```
./OpenZenExample
Listing sensors:
[2022-07-16 11:02:49.362] [OpenZen_console] [info] Starting listing of Bluetooth devices
0: LPMSB2-6F377E (Bluetooth)
Provide an index within the range 0-0:
Note that the default connection baud rate is 921600, which is not the case for LPMS-BE/ME sensors. 
More details in the comment of this program.
```

7. `ExamplePython.py`
Open a new terminal (or tab)
``` 
cd /openzen
conda activate *VE 
export PYTHONPATH=$HOME/repositories/openzen/build
python hello-sensor.py
```


## References 
https://bitbucket.org/lpresearch/openzen/src/master/   
https://github.com/xfetus/pe/tree/main/hardware/sensors/imus-LPMS-B2/adquistion-software/ros/openzen  
https://lpresearch.bitbucket.io/openzen/latest/setup.html
