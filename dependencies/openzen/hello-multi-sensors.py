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

s0MACid = "00:04:3E:6F:37:7E"
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
error, sensor_s7e = client.obtain_sensor_by_name("Bluetooth", s0MACid)
if not error == openzen.ZenError.NoError:
    print("Error connecting to sensor", s0MACid)
    sys.exit(1)
imu_s7e = sensor_s7e.get_any_component_of_type(openzen.component_type_imu)

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


# Set stream frequency
streamFreq = 100 # Hz

error = imu_s7e.set_int32_property(openzen.ZenImuProperty.SamplingRate, streamFreq)
error, freq = imu_s7e.get_int32_property(openzen.ZenImuProperty.SamplingRate)
print("Sampling rate imu_s7e: {}".format(freq))

error = imu_s5b.set_int32_property(openzen.ZenImuProperty.SamplingRate, streamFreq)
error, freq = imu_s5b.get_int32_property(openzen.ZenImuProperty.SamplingRate)
print("Sampling rate imu_s5b: {}".format(freq))

error = imu_s95.set_int32_property(openzen.ZenImuProperty.SamplingRate, streamFreq)
error, freq = imu_s95.get_int32_property(openzen.ZenImuProperty.SamplingRate)
print("Sampling rate imu_s95: {}".format(freq))


##############################################################
print(f"-----------------------")
print(f'Sensor sync')
imu_s7e.execute_property(openzen.ZenImuProperty.StartSensorSync)
imu_s95.execute_property(openzen.ZenImuProperty.StartSensorSync)
imu_s5b.execute_property(openzen.ZenImuProperty.StartSensorSync)

## wait for 3 seconds for the syn commands to arrive
time.sleep(3)

# Clear existing imu data in event queue
while client.poll_next_event():
    pass


print(f"-----------------------")
# set both sensors back to normal mode
imu_s7e.execute_property(openzen.ZenImuProperty.StopSensorSync)
imu_s95.execute_property(openzen.ZenImuProperty.StopSensorSync)
imu_s5b.execute_property(openzen.ZenImuProperty.StopSensorSync)
print(f'Sensor sync completed ')

total_number_of_samples = streamFreq * 10 # Collect 10 seconds of data
imu_s7e_data_count = 0
imu_s5b_data_count = 0
imu_s95_data_count = 0

while True:
    zenEvent = client.wait_for_next_event()

    print('\n --------------------   \n ')

    print('imu_s7e > ')
    if zenEvent.event_type == openzen.ZenEventType.ImuData and \
            zenEvent.sensor == imu_s7e.sensor and \
            zenEvent.component.handle == imu_s7e.component.handle:
        imu_data = zenEvent.data.imu_data

        imu_s7e_data_count = imu_s7e_data_count + 1
        print(f'  imu_s7e {imu_s7e_data_count}, {imu_data.timestamp}, {imu_data.q}')

    print('imu_s5b > ')
    if zenEvent.event_type == openzen.ZenEventType.ImuData and \
            zenEvent.sensor == imu_s5b.sensor and \
            zenEvent.component.handle == imu_s5b.component.handle:
        imu_data = zenEvent.data.imu_data

        imu_s5b_data_count = imu_s5b_data_count + 1
        print(f'  imu_s5b {imu_s5b_data_count}, {imu_data.timestamp}, {imu_data.q}')

    print('imu_s95 > ')
    if zenEvent.event_type == openzen.ZenEventType.ImuData and \
            zenEvent.sensor == imu_s95.sensor and \
            zenEvent.component.handle == imu_s95.component.handle:
        imu_data = zenEvent.data.imu_data

        imu_s95_data_count = imu_s95_data_count + 1
        print(f'  imu_s95 {imu_s95_data_count}, {imu_data.timestamp}, {imu_data.q}')

    # Check data count of 1 sensor as loop termination condition for easier comparison
    if imu_s5b_data_count >= total_number_of_samples:
        break

print("Streaming of sensor data complete")


print(f"-----------------------")
sensor_s5b.release()
sensor_s95.release()
client.close()
print("OpenZen library was closed")
