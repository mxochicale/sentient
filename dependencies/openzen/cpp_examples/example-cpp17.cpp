#define OPENZEN_CXX17
#include <OpenZen.h>

#include <iostream>

using namespace zen;

int main(int argc, char* argv[])
{
    // enable resonable log output for OpenZen
    ZenSetLogLevel(ZenLogLevel_Info);

    // create OpenZen Clien
    auto [clientError, client] = make_client();

    if (clientError) {
        std::cout << "Cannot create OpenZen client" << std::endl;
        return clientError;
    }

    // connect to sensor on IO System by the sensor name
    auto [obtainError, sensor] = client.obtainSensorByName("Bluetooth", "00:04:3E:53:ED:5B", 921600);
    if (obtainError)
    {
        std::cout << "Cannot connect to sensor" << std::endl;
        client.close();
        return obtainError;
    }

    // check that the sensor has an IMU component
    auto imu = sensor.getAnyComponentOfType(g_zenSensorType_Imu);

    if (!imu)
    {
        std::cout << "Connected sensor has no IMU" << std::endl;
        client.close();
        return ZenError_WrongSensorType;
    }

    // set and get current streaming frequency
    auto error = imu->setInt32Property(ZenImuProperty_SamplingRate, 50);
    if (error) {
        std::cout << "Error setting streaming frequency" << std::endl;
        client.close();
        return error;
    }

    auto freqPair = imu->getInt32Property(ZenImuProperty_SamplingRate);
    if (freqPair.first) {
        std::cout << "Error fetching streaming frequency" << std::endl;
        client.close();
        return freqPair.first;
    }
    std::cout << "Streaming frequency: " << freqPair.second << std::endl;

    // toggle on/off of a particular data output (linAcc is not ON by default)
    error = imu->setBoolProperty(ZenImuProperty_OutputLinearAcc, true);
    if (error) {
        std::cout << "Error toggling ON linear acc data output" << std::endl;
        client.close();
        return error;
    }

    // readout up to 100 samples from the IMU
    // note that there are 2 gyro fields in the IMU data structure (ZenImuData struct in include/ZenTypes.h)
    // please refer to your sensor's manual for correct retrieval of gyro data
    for (int i = 0; i < 100; i++) {
        auto event = client.waitForNextEvent();
        if (event->component.handle == imu->component().handle) {

            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << std::endl;
            std::cout << "> Timestamp [s] \t\t " << event->data.imuData.timestamp << std::endl;

            std::cout << "> Acceleration [g] \t\t x = " << event->data.imuData.a[0]
                << "\t y = " << event->data.imuData.a[1]
                << "\t z = " << event->data.imuData.a[2] << std::endl;
            
            // depending on sensor, gyro data is outputted to g1, g2 or both
            // read more on https://lpresearch.bitbucket.io/openzen/latest/getting_started.html#id1
            std::cout << "> Gyro 1 [deg/seg] \t\t x = " << event->data.imuData.g1[0]
                << "\t y = " << event->data.imuData.g1[1]
                << "\t z = " << event->data.imuData.g1[2] << std::endl;

            std::cout << "> Magnetometer [uT] \t\t x = " << event->data.imuData.b[0]
                << "\t y = " << event->data.imuData.b[1]
                << "\t z = " << event->data.imuData.b[2] << std::endl;

            std::cout << "> Euler [deg/s] \t\t x = " << event->data.imuData.r[0]
                << "\t y = " << event->data.imuData.r[1]
                << "\t z = " << event->data.imuData.r[2] << std::endl;

            std::cout << "> Quaternions [nounit] \t\t q0 = " << event->data.imuData.q[0] 
                << "\t q1 = " << event->data.imuData.q[1]
                << "\t q2 = " << event->data.imuData.q[2]
                << "\t q3 = " << event->data.imuData.q[3] << std::endl;

            std::cout << "> Rotation Matrix [nounit] \t rM[0] = " << event->data.imuData.rotationM[0]
                << "\t rM[1] = " << event->data.imuData.rotationM[1]
                << "\t rM[2] = " << event->data.imuData.rotationM[2]
                << "\t rM[3] = " << event->data.imuData.rotationM[3]
                << "\t rM[4] = " << event->data.imuData.rotationM[4]
                << "\t rM[5] = " << event->data.imuData.rotationM[5]
                << "\t rM[6] = " << event->data.imuData.rotationM[6]
                << "\t rM[7] = " << event->data.imuData.rotationM[7]
                << "\t rM[8] = " << event->data.imuData.rotationM[8]
                << "\t rM[9] = " << event->data.imuData.rotationM[9]
                << "\t rM[10] = " << event->data.imuData.rotationM[10]
                << "\t rM[11] = " << event->data.imuData.rotationM[11]
                << "\t rM[12] = " << event->data.imuData.rotationM[12]
                << "\t rM[13] = " << event->data.imuData.rotationM[13]
                << "\t rM[14] = " << event->data.imuData.rotationM[14]
                << "\t rM[15] = " << event->data.imuData.rotationM[15]
                << "\t rM[16] = " << event->data.imuData.rotationM[16]
                << "\t rM[17] = " << event->data.imuData.rotationM[17] << std::endl;

            std::cout << "> Rotation Matrix Offset [nounit] \t roM[0] = " << event->data.imuData.rotOffsetM[0]
                << "\t rotM[1] = " << event->data.imuData.rotOffsetM[1]
                << "\t roM[2] = " << event->data.imuData.rotOffsetM[2]
                << "\t roM[3] = " << event->data.imuData.rotOffsetM[3]
                << "\t roM[4] = " << event->data.imuData.rotOffsetM[4]
                << "\t roM[5] = " << event->data.imuData.rotOffsetM[5]
                << "\t roM[6] = " << event->data.imuData.rotOffsetM[6]
                << "\t roM[7] = " << event->data.imuData.rotOffsetM[7]
                << "\t roM[8] = " << event->data.imuData.rotOffsetM[8]
                << "\t roM[9] = " << event->data.imuData.rotOffsetM[9]
                << "\t roM[10] = " << event->data.imuData.rotOffsetM[10]
                << "\t roM[11] = " << event->data.imuData.rotOffsetM[11]
                << "\t roM[12] = " << event->data.imuData.rotOffsetM[12]
                << "\t roM[13] = " << event->data.imuData.rotOffsetM[13]
                << "\t roM[14] = " << event->data.imuData.rotOffsetM[14]
                << "\t roM[15] = " << event->data.imuData.rotOffsetM[15]
                << "\t roM[16] = " << event->data.imuData.rotOffsetM[16]
                << "\t roM[17] = " << event->data.imuData.rotOffsetM[17] << std::endl;

        }
    }

    client.close();
    std::cout << "Sensor connection closed" << std::endl;
    return 0;
}

