###########################################################################
#
# hello-sensor.py: OpenZen Python example
#
# Make sure the openzen.pyd (for Windows) or openzen.so (Linux/Mac)
# are in the same folder as this file.
# If you want to connect to USB sensors on Windows, the file SiUSBXp.dll
# should also be in the same folder.
#
# Reference:
# https://bitbucket.org/lpresearch/openzen/src/master/examples/ExamplePython.py
###########################################################################

import sys
import openzen

### FOR GNU/LINUX OS
## set PYTHONPATH to find OpenZen python module
## export PYTHONPATH=$HOME/repositories/openzen/build
#from libOpenZen import openzen
### FOR WINDOWS OS
## sys.path.append("C:/Users/$MACHINE/$USERNAME/$PATH_OF_THE_REPO/$PATH_OF_THE_SCRIPT")

openzen.set_log_level(openzen.ZenLogLevel.Warning)

error, client = openzen.make_client()
if not error == openzen.ZenError.NoError:
    print("Error while initializing OpenZen library")
    sys.exit(1)

error = client.list_sensors_async()

# check for events
sensor_desc_connect = None
while True:
    zenEvent = client.wait_for_next_event()

    if zenEvent.event_type == openzen.ZenEventType.SensorFound:
        print("Found sensor {} on IoType {}".format(zenEvent.data.sensor_found.name,
                                                    zenEvent.data.sensor_found.io_type))
        if sensor_desc_connect is None:
            sensor_desc_connect = zenEvent.data.sensor_found

    if zenEvent.event_type == openzen.ZenEventType.SensorListingProgress:
        lst_data = zenEvent.data.sensor_listing_progress
        print("Sensor listing progress: {} %".format(lst_data.progress * 100))
        if lst_data.complete > 0:
            break
print("Sensor Listing complete")

if sensor_desc_connect is None:
    print("No sensors found")
    sys.exit(1)

# connect to the first sensor found
# error, sensor = client.obtain_sensor(sensor_desc_connect)

# or connect to a sensor by name
#error, sensor = client.obtain_sensor_by_name("LinuxDevice", "LPMSCU2000003")
#error, sensor = client.obtain_sensor_by_name("Bluetooth", "00:04:3E:53:ED:5B", 921600)
#error, sensor = client.obtain_sensor_by_name("Bluetooth", "00:04:3E:6F:37:95", 921600)
error, sensor = client.obtain_sensor_by_name("Bluetooth", "00:04:3E:6F:37:7E", 921600) #LPMSB2-6F377E

if not error == openzen.ZenSensorInitError.NoError:
    print("Error connecting to sensor")
    sys.exit(1)

print("Connected to sensor !")

imu = sensor.get_any_component_of_type(openzen.component_type_imu)
if imu is None:
    print("No IMU found")
    sys.exit(1)

## read bool property
error, is_streaming = imu.get_bool_property(openzen.ZenImuProperty.StreamData)
if not error == openzen.ZenError.NoError:
    print("Can't load streaming settings")
    sys.exit(1)

print("Sensor is streaming data: {}".format(is_streaming))

print("\n>> Set and get IMU settings")

##############################################################
print(f'\nSet and get IMU settings')
error = imu.set_int32_property(openzen.ZenImuProperty.Id, 6)
error, imu_id = imu.get_int32_property(openzen.ZenImuProperty.Id)
print("IMU ID: {}".format(imu_id))

print(f'\n Test to set freq')
error = imu.set_int32_property(openzen.ZenImuProperty.SamplingRate, 400)
error, freq = imu.get_int32_property(openzen.ZenImuProperty.SamplingRate)
print("Sampling rate: {}".format(freq))

##############################################################
print(f'\n Load the alignment matrix from the sensor')
error, accAlignment = imu.get_array_property_float(openzen.ZenImuProperty.AccAlignment)
if not error == openzen.ZenError.NoError:
   print ("Can't load alignment")
   sys.exit(1)

if not len(accAlignment) == 9:
   print ("Loaded Alignment has incosistent size")
   sys.exit(1)

print ("Alignment loaded: {}".format(accAlignment))

## store float array
error = imu.set_array_property_float(openzen.ZenImuProperty.AccAlignment, accAlignment)

if not error == openzen.ZenError.NoError:
   print ("Can't store alignment")
   sys.exit(1)

print("Stored alignment {} to sensor".format(accAlignment))


##############################################################
# start streaming data
runSome = 0
total_number_of_samples = 500

while True:
    zenEvent = client.wait_for_next_event()

    # check if its an IMU sample event and if it
    # comes from our IMU and sensor component
    if zenEvent.event_type == openzen.ZenEventType.ImuData and \
            zenEvent.sensor == imu.sensor and \
            zenEvent.component.handle == imu.component.handle:
        imu_data = zenEvent.data.imu_data
        print(f'    --------------------------------------------')
        print(f'        Timestamp [s]: {imu_data.timestamp}')
        print(f'            A [m/s^2]: {imu_data.a}')
        print(f' Quaternion [no unit]: {imu_data.q}')
        print(f'        Euler [deg/s]: {imu_data.r}')
        print(f'            G [deg/s]: {imu_data.g1} ')
        # See more data descriptions
        # https://lpresearch.bitbucket.io/openzen/latest/data.html

    runSome = runSome + 1
    if runSome > total_number_of_samples:
        break

print("Streaming of sensor data complete")
sensor.release()
client.close()
print("OpenZen library was closed")
