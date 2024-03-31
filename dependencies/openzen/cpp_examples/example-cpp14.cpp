#define OPENZEN_CXX14
#include <OpenZen.h>
#include <iostream>

using namespace zen;

int main(int argc, char* argv[])
{
    // enable resonable log output for OpenZen
    ZenSetLogLevel(ZenLogLevel_Info);

    // create OpenZen Clien
    auto clientPair = make_client();
    auto& clientError = clientPair.first;
    auto& client = clientPair.second;

    if (clientError) {
        std::cout << "OpenZen client could not be created" << std::endl;
        return clientError;
    }

    std::cout << "OpenZen client created successfully" << std::endl;

    // connect to sensor on IO System by the sensor name
    auto sensorPair = client.obtainSensorByName("Bluetooth", "00:04:3E:53:ED:5B", 921600);
    auto& obtainError = sensorPair.first;
    auto& sensor = sensorPair.second;
    if (obtainError)
    {
        std::cout << "Cannot connect to sensor" << std::endl;
        client.close();
        return obtainError;
    }

    // check that the sensor has an IMU component
    auto imuPair = sensor.getAnyComponentOfType(g_zenSensorType_Imu);
    auto& hasImu = imuPair.first;
    auto imu = imuPair.second;

    if (!hasImu)
    {
        std::cout << "Connected sensor has no IMU" << std::endl;
        client.close();
        return ZenError_WrongSensorType;
    }

    // set and get current streaming frequency
    auto error = imu.setInt32Property(ZenImuProperty_SamplingRate, 50);
    if (error) {
        std::cout << "Error setting streaming frequency" << std::endl;
        client.close();
        return error;
    }

    auto freqPair = imu.getInt32Property(ZenImuProperty_SamplingRate);
    if (freqPair.first) {
        std::cout << "Error fetching streaming frequency" << std::endl;
        client.close();
        return freqPair.first;
    }
    std::cout << "Streaming frequency: " << freqPair.second << std::endl;

    // toggle on/off of a particular data output (linAcc is not ON by default)
    error = imu.setBoolProperty(ZenImuProperty_OutputLinearAcc, true);
    if (error) {
        std::cout << "Error toggling ON linear acc data output" << std::endl;
        client.close();
        return error;
    }

    // readout up to 100 samples from the IMU
    // note that there are 2 gyro fields in the IMU data structure (ZenImuData struct in include/ZenTypes.h)
    // please refer to your sensor's manual for correct retrieval of gyro data https://lpresearch.bitbucket.io/openzen/latest/data.html
    for (int i = 0; i < 100; i++) {
        auto event = client.waitForNextEvent();
        if (event.second.component.handle == imu.component().handle) {

            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << std::endl;
            std::cout << "> Timestamp [s] \t\t " << event.second.data.imuData.timestamp << std::endl;

            std::cout << "> Acceleration [g] \t\t x = " << event.second.data.imuData.a[0]
                << "\t y = " << event.second.data.imuData.a[1]
                << "\t z = " << event.second.data.imuData.a[2] << std::endl;

            std::cout << "> Gyro 1 [deg/seg] \t\t x = " << event.second.data.imuData.g1[0]
                << "\t y = " << event.second.data.imuData.g1[1]
                << "\t z = " << event.second.data.imuData.g1[2] << std::endl;

            std::cout << "> Magnetometer [uT] \t\t x = " << event.second.data.imuData.b[0]
                << "\t y = " << event.second.data.imuData.b[1]
                << "\t z = " << event.second.data.imuData.b[2] << std::endl;

            std::cout << "> Euler [deg/s] \t\t x = " << event.second.data.imuData.r[0]
                << "\t y = " << event.second.data.imuData.r[1]
                << "\t z = " << event.second.data.imuData.r[2] << std::endl;

            std::cout << "> Quaternions \t\t\t q0 = " << event.second.data.imuData.q[0]
                << "\t q1 = " << event.second.data.imuData.q[1]
                << "\t q2 = " << event.second.data.imuData.q[2]
                << "\t q3 = " << event.second.data.imuData.q[3] << std::endl;

            std::cout << "> Rotation Matrix [nounit] \t rM[0] = " << event.second.data.imuData.rotationM[0]
                << "\t rM[1] = " << event.second.data.imuData.rotationM[1]
                << "\t rM[2] = " << event.second.data.imuData.rotationM[2]
                << "\t rM[3] = " << event.second.data.imuData.rotationM[3]
                << "\t rM[4] = " << event.second.data.imuData.rotationM[4]
                << "\t rM[5] = " << event.second.data.imuData.rotationM[5]
                << "\t rM[6] = " << event.second.data.imuData.rotationM[6]
                << "\t rM[7] = " << event.second.data.imuData.rotationM[7]
                << "\t rM[8] = " << event.second.data.imuData.rotationM[8]
                << "\t rM[9] = " << event.second.data.imuData.rotationM[9]
                << "\t rM[10] = " << event.second.data.imuData.rotationM[10]
                << "\t rM[11] = " << event.second.data.imuData.rotationM[11]
                << "\t rM[12] = " << event.second.data.imuData.rotationM[12]
                << "\t rM[13] = " << event.second.data.imuData.rotationM[13]
                << "\t rM[14] = " << event.second.data.imuData.rotationM[14]
                << "\t rM[15] = " << event.second.data.imuData.rotationM[15]
                << "\t rM[16] = " << event.second.data.imuData.rotationM[16]
                << "\t rM[17] = " << event.second.data.imuData.rotationM[17] << std::endl;

            std::cout << "> Rotation Matrix Offset [nounit] \t roM[0] = " << event.second.data.imuData.rotOffsetM[0]
                << "\t rotM[1] = " << event.second.data.imuData.rotOffsetM[1]
                << "\t roM[2] = " << event.second.data.imuData.rotOffsetM[2]
                << "\t roM[3] = " << event.second.data.imuData.rotOffsetM[3]
                << "\t roM[4] = " << event.second.data.imuData.rotOffsetM[4]
                << "\t roM[5] = " << event.second.data.imuData.rotOffsetM[5]
                << "\t roM[6] = " << event.second.data.imuData.rotOffsetM[6]
                << "\t roM[7] = " << event.second.data.imuData.rotOffsetM[7]
                << "\t roM[8] = " << event.second.data.imuData.rotOffsetM[8]
                << "\t roM[9] = " << event.second.data.imuData.rotOffsetM[9]
                << "\t roM[10] = " << event.second.data.imuData.rotOffsetM[10]
                << "\t roM[11] = " << event.second.data.imuData.rotOffsetM[11]
                << "\t roM[12] = " << event.second.data.imuData.rotOffsetM[12]
                << "\t roM[13] = " << event.second.data.imuData.rotOffsetM[13]
                << "\t roM[14] = " << event.second.data.imuData.rotOffsetM[14]
                << "\t roM[15] = " << event.second.data.imuData.rotOffsetM[15]
                << "\t roM[16] = " << event.second.data.imuData.rotOffsetM[16]
                << "\t roM[17] = " << event.second.data.imuData.rotOffsetM[17] << std::endl;

        }
    }

    client.close();
    std::cout << "OpenZen client closed successfully" << std::endl;
    return 0;
}

