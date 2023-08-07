# Sensor fusion

## Setting up paths and VE env
```
cd $HOME/repositories/rtt4ssa/rtt4ssa/sensor_fusion
export PYTHONPATH=$HOME/repositories/openzen/build
mamba activate rtt4ssaVE
```

## Collecting data
1. Setting up IMU  
1.1 Either plugin bluetooth dongle or activate bluetooth
1.2. (Optional)  Run commands to know mac addresses 
```
hcitool dev
Devices:
    hci0	00:30:91:10:00:26

hcitool scan
Scanning ...
	00:04:3E:6F:37:7E	LPMSB2-6F377E
	00:04:3E:53:ED:58	LPMSB2-53ED58
```
1.3 (Optional) Test data streaming 

```
cd $HOME/repositories/rtt4ssa/dependencies/openzen
python hello-sensor.py
```

2. Setting up usb-endoscope camera  
Connect USB endoscope camera, you can test camera with scripts in [usb_endoscope](../usb_endoscope)

3. Recording data
3.1 Switch on sensor and scan ports
```
hcitool scan
Scanning ...
	00:04:3E:53:ED:58	LPMSB2-53ED58
```
3.2 Connect usb endoscope camera and quickly test port id
```
cd $HOME/repositories/rtt4ssa/rtt4ssa/usb_endoscope
python simple_record_video.py --vfn 'simpleCapturedVideo.mp4' --idFG 4
vlc simpleCapturedVideo.mp4
```

3.3 Run command with the desired image resolution and press Q to quick script
```
cd $HOME/repositories/rtt4ssa/rtt4ssa/sensor_fusion
python video_usb_imu_bluetooth.py --idFG 4 --fps 120 --fW 160 --fH 120 --vfn testNN.avi
python video_usb_imu_bluetooth.py --idFG 4 --fps 120 --fW 320 --fH 240 --vfn testNN.avi
vlc testNN.avi
```

3.4 For two sensors 
``` 
cd $HOME/repositories/rtt4ssa/rtt4ssa/sensor_fusion
python video_usb_2imus_bluetooth.py --idFG 4 --fps 120 --fW 160 --fH 120 --vfn test2imus_NN.avi
vlc test2imus_NN.avi
```
You might need to kill the process 
```
ps -ef | grep "python video_usb_2imus_bluetooth.py" | awk '{print $2}' | xargs kill
```

3.5 Switch off IMU sensors and disconnect camera!