# Sensor fusion

## Setting up paths and VE env
```
cd $HOME/repositories/in2research2023/scripts/sensor-fusion
export PYTHONPATH=$HOME/repositories/openzen/build
conda activate ai-surg-skills-VE
```

## Collecting data
1. Setting up IMU  
1.1 Either plugin bluetooth dongle or activate bluetooth
1.2. Run commands to know mac addresses 
```
hcitool dev
Devices:
    hci0	00:30:91:10:00:26

hcitool scan
Scanning ...
	00:04:3E:6F:37:7E	LPMSB2-6F377E
	00:04:3E:53:ED:58	LPMSB2-53ED58
```
1.3 Test data streaming 

```
cd $HOME/repositories/in2research2023/dependencies/openzen
python hello-sensor.py
```

2. Setting up usb-endoscope camera  
Connect USB endoscope camera
2.1 Test camera
```
cd $HOME/repositories/in2research2023/scripts/usb-endoscope

python capturing_video.py --idFG 4 --fW 320 --fH 240 --FPS 15 --buffer_size 1
#Press Q to exit(quit)
```

3. Recording data
3.1 Switch on sensor and scan ports
```
hcitool scan
Scanning ...
	00:04:3E:53:ED:58	LPMSB2-53ED58
```
3.2 Connect usb endoscope camera
3.3 Run command with the desired image resoltion and press Q to quick script
```
cd $HOME/repositories/in2research2023/scripts/sensor-fusion
python video-usb-imu-bluetooth.py --idFG 4 --fps 30 --fW 160 --fH 120 --vfn testNN.avi
python video-usb-imu-bluetooth.py --idFG 4 --fps 30 --fW 320 --fH 240 --vfn testNN.avi
```
3.4 Swith off IMU sensor and disconnect camera!


