#define OPENZEN_CXX17
#include <OpenZen.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <chrono>     // for steady_clock
#include <algorithm>  // for std::min_element / std::max_element
#include <numeric>    // for std::accumulate
#include <cmath>      // for std::sqrt

using namespace zen;

int main(int argc, char* argv[])
{
    // Default values
    const char* macAddress = "00:04:3E:53:ED:5B";
    int samplingRate = 50;
    int baudRate = 921600;
    bool verbose = false;

    // Collect positional arguments, consuming any --verbose flags
    // Usage: program [--verbose] [MAC_ADDRESS] [SAMPLING_RATE] [BAUD_RATE]
    std::vector<const char*> positional;
    positional.reserve(static_cast<size_t>(argc));
    positional.push_back(argv[0]); // program name (unused for parsing)
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else {
            positional.push_back(argv[i]);
        }
    }

    if (positional.size() > 1) {
        macAddress = positional[1];
    }
    if (positional.size() > 2) {
        char* endptr = nullptr;
        long val = std::strtol(positional[2], &endptr, 10);
        if (endptr == positional[2] || *endptr != '\0' || val <= 0) {
            std::cout << "Invalid sampling rate: " << positional[2] << std::endl;
            return 1;
        }
        samplingRate = static_cast<int>(val);
    }
    if (positional.size() > 3) {
        char* endptr = nullptr;
        long val = std::strtol(positional[3], &endptr, 10);
        if (endptr == positional[3] || *endptr != '\0' || val <= 0) {
            std::cout << "Invalid baud rate: " << positional[3] << std::endl;
            return 1;
        }
        baudRate = static_cast<int>(val);
    }

    std::cout << "Using MAC address: " << macAddress << std::endl;
    std::cout << "Using sampling rate: " << samplingRate << " Hz" << std::endl;
    std::cout << "Using baud rate: " << baudRate << std::endl;
    std::cout << "Verbose output: " << (verbose ? "on" : "off") << std::endl;

    // enable resonable log output for OpenZen
    ZenSetLogLevel(ZenLogLevel_Info);

    // create OpenZen Clien
    auto [clientError, client] = make_client();

    if (clientError) {
        std::cout << "Cannot create OpenZen client" << std::endl;
        return clientError;
    }

    // connect to sensor on IO System by the sensor name
    auto [obtainError, sensor] = client.obtainSensorByName("Bluetooth", macAddress, baudRate);
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
    auto error = imu->setInt32Property(ZenImuProperty_SamplingRate, samplingRate);
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

    // ------------------------------------------------------------------
    // NEW: timing instrumentation
    // ------------------------------------------------------------------
    using Clock = std::chrono::steady_clock;
    std::vector<double> intervalsMs;  // wall-clock delta between IMU events
    intervalsMs.reserve(128);

    Clock::time_point prevEventTime{};
    Clock::time_point firstEventTime{};
    bool havePrev = false;

    // readout up to 100 samples from the IMU
    int samples = 0;
    while (samples < 100) {
        auto event = client.waitForNextEvent();
        if (!event) break;
        if (event->component.handle != imu->component().handle) continue;

        // NEW: timestamp arrival of this IMU sample
        auto now = Clock::now();
        if (!havePrev) {
            firstEventTime = now;
            havePrev = true;
        } else {
            double dtMs = std::chrono::duration<double, std::milli>(now - prevEventTime).count();
            intervalsMs.push_back(dtMs);
        }
        prevEventTime = now;

        // ... process
        if (verbose) {
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
        } else {
            // lightweight progress indicator for non-verbose runs
            std::cout << '.' << std::flush;
        }

        ++samples;
    }

    if (!verbose) std::cout << std::endl;

    // ------------------------------------------------------------------
    // NEW: timing summary
    // ------------------------------------------------------------------
    if (samples > 1 && !intervalsMs.empty()) {
        double totalMs = std::chrono::duration<double, std::milli>(
            prevEventTime - firstEventTime).count();

        double minMs = *std::min_element(intervalsMs.begin(), intervalsMs.end());
        double maxMs = *std::max_element(intervalsMs.begin(), intervalsMs.end());
        double sumMs = std::accumulate(intervalsMs.begin(), intervalsMs.end(), 0.0);
        double meanMs = sumMs / static_cast<double>(intervalsMs.size());

        // Observed rate from wall clock over the whole window
        double observedHz = (samples - 1) * 1000.0 / totalMs;

        // RMS jitter around the mean interval
        double sq = 0.0;
        for (double v : intervalsMs) {
            double d = v - meanMs;
            sq += d * d;
        }
        double rmsJitterMs = std::sqrt(sq / static_cast<double>(intervalsMs.size()));

        std::cout << "\n--- Timing summary (wall clock) ---\n";
        std::cout << "Samples:               " << samples << "\n";
        std::cout << "Elapsed:               " << totalMs << " ms\n";
        std::cout << "Observed rate:         " << observedHz << " Hz"
                  << "  (requested " << samplingRate << " Hz)\n";
        std::cout << "Mean interval:         " << meanMs << " ms  ("
                  << (meanMs > 0.0 ? 1000.0 / meanMs : 0.0) << " Hz)\n";
        std::cout << "Min / Max interval:    " << minMs << " / " << maxMs << " ms\n";
        std::cout << "RMS jitter:            " << rmsJitterMs << " ms\n";
    }

    client.close();
    std::cout << "Sensor connection closed" << std::endl;
    std::cout << "Received " << samples << " IMU samples\n";
    return 0;
}