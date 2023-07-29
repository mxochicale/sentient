###########################################################################
#
# hello-multi-sensor.py: OpenZen Python example
#
# References:
# https://bitbucket.org/lpresearch/openzen/src/master/examples/ExamplePython.py
# https://lpresearch.bitbucket.io/openzen/latest/examples.html#synchronizing-multiple-sensors
###########################################################################

import sys
import time

import openzen

## FOR GNU/LINUX OS
## set PYTHONPATH to find OpenZen python module
## export PYTHONPATH=$HOME/repositories/openzen/build

s1MACid = "00:04:3E:6F:37:95"
s2MACid = "00:04:3E:53:ED:5B"
print(f"-----------------------")
print(f"Sensor Listing")

openzen.set_log_level(openzen.ZenLogLevel.Warning)

error, client = openzen.make_client()
if not error == openzen.ZenError.NoError:
    print("Error while initializing OpenZen library")
    sys.exit(1)

error = client.list_sensors_async()

#############################################################
print(f'Connecting to sensors')
error, sensor_s95 = client.obtain_sensor_by_name("Bluetooth", s1MACid)
if not error == openzen.ZenError.NoError:
    print("Error connecting to sensor", s1MACid)
    sys.exit(1)
imu_s95 = sensor_s95.get_any_component_of_type(openzen.component_type_imu)

error, sensor_s5b = client.obtain_sensor_by_name("Bluetooth", s2MACid)
if not error == openzen.ZenError.NoError:
    print("Error connecting to sensor", s2MACid)
    sys.exit(1)
imu_s5b = sensor_s5b.get_any_component_of_type(openzen.component_type_imu)
print(f'Sensors connected')

##############################################################
print(f"-----------------------")
print(f'Sensor sync')
imu_s95.execute_property(openzen.ZenImuProperty.StartSensorSync)
imu_s5b.execute_property(openzen.ZenImuProperty.StartSensorSync)

## wait for 3 seconds for the syn commands to arrive
time.sleep(3)

print(f"-----------------------")
# set both sensors back to normal mode
imu_s95.execute_property(openzen.ZenImuProperty.StopSensorSync)
imu_s5b.execute_property(openzen.ZenImuProperty.StopSensorSync)
print(f'Sensor sync completed ')

##############################################################
print(f"-----------------------")
print(f'Streaming data....')
runSome = 0
total_number_of_samples = 50

while True:
    zenEvent = client.wait_for_next_event()

    print('printing imu data from imu_s5b')
    if zenEvent.event_type == openzen.ZenEventType.ImuData and \
            zenEvent.sensor == imu_s5b.sensor and \
            zenEvent.component.handle == imu_s5b.component.handle:
        imu_data = zenEvent.data.imu_data
        print(f'        imu_s5b')
        print(f'        Timestamp [s]: {imu_data.timestamp}')
        print(f'        Quaternion [no unit]: {imu_data.q}')

    print('printing imu data from imu_s95')
    if zenEvent.event_type == openzen.ZenEventType.ImuData and \
            zenEvent.sensor == imu_s95.sensor and \
            zenEvent.component.handle == imu_s95.component.handle:
        imu_data = zenEvent.data.imu_data
        print(f'        imu_s95')
        print(f'        Timestamp [s]: {imu_data.timestamp}')
        print(f'        Quaternion [no unit]: {imu_data.q}')

    runSome = runSome + 1
    if runSome > total_number_of_samples:
        break

print("Streaming of sensor data complete")


print(f"-----------------------")
sensor_s5b.release()
sensor_s95.release()
client.close()
print("OpenZen library was closed")
