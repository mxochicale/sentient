#define OPENZEN_CXX17
#include <OpenZen.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <thread>

using namespace zen;

int main(int argc, char* argv[])
{
    // Default values
    const char* macAddressA = "00:04:3E:6F:37:7E";
    const char* macAddressB = "00:04:3E:53:ED:5B";
    const char* macAddressC = "00:04:3E:6F:37:95";
    int samplingRate = 50;
    int baudRate = 921600;
    bool verbose = false;

    // Usage: program [--verbose] [MAC_A] [MAC_B] [MAC_C] [SAMPLING_RATE] [BAUD_RATE]
    std::vector<const char*> positional;
    positional.reserve(static_cast<size_t>(argc));
    positional.push_back(argv[0]);
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else {
            positional.push_back(argv[i]);
        }
    }

    if (positional.size() > 1) macAddressA = positional[1];
    if (positional.size() > 2) macAddressB = positional[2];
    if (positional.size() > 3) macAddressC = positional[3];
    if (positional.size() > 4) {
        char* endptr = nullptr;
        long val = std::strtol(positional[4], &endptr, 10);
        if (endptr == positional[4] || *endptr != '\0' || val <= 0) {
            std::cout << "Invalid sampling rate: " << positional[4] << std::endl;
            return 1;
        }
        samplingRate = static_cast<int>(val);
    }
    if (positional.size() > 5) {
        char* endptr = nullptr;
        long val = std::strtol(positional[5], &endptr, 10);
        if (endptr == positional[5] || *endptr != '\0' || val <= 0) {
            std::cout << "Invalid baud rate: " << positional[5] << std::endl;
            return 1;
        }
        baudRate = static_cast<int>(val);
    }

    std::cout << "Sensor A MAC: " << macAddressA << std::endl;
    std::cout << "Sensor B MAC: " << macAddressB << std::endl;
    std::cout << "Sensor C MAC: " << macAddressC << std::endl;
    std::cout << "Sampling rate: " << samplingRate << " Hz" << std::endl;
    std::cout << "Baud rate: " << baudRate << std::endl;
    std::cout << "Verbose output: " << (verbose ? "on" : "off") << std::endl;

    ZenSetLogLevel(ZenLogLevel_Info);

    auto [clientError, client] = make_client();
    if (clientError) {
        std::cout << "Cannot create OpenZen client" << std::endl;
        return clientError;
    }

    // --- Connect to Sensor A ---
    auto [obtainErrorA, sensorA] = client.obtainSensorByName("Bluetooth", macAddressA, baudRate);
    if (obtainErrorA) {
        std::cout << "Cannot connect to sensor A" << std::endl;
        client.close();
        return obtainErrorA;
    }
    auto imuA = sensorA.getAnyComponentOfType(g_zenSensorType_Imu);
    if (!imuA) {
        std::cout << "Sensor A has no IMU" << std::endl;
        client.close();
        return ZenError_WrongSensorType;
    }

    // --- Connect to Sensor B ---
    auto [obtainErrorB, sensorB] = client.obtainSensorByName("Bluetooth", macAddressB, baudRate);
    if (obtainErrorB) {
        std::cout << "Cannot connect to sensor B" << std::endl;
        client.close();
        return obtainErrorB;
    }
    auto imuB = sensorB.getAnyComponentOfType(g_zenSensorType_Imu);
    if (!imuB) {
        std::cout << "Sensor B has no IMU" << std::endl;
        client.close();
        return ZenError_WrongSensorType;
    }

    // --- Connect to Sensor C ---
    auto [obtainErrorC, sensorC] = client.obtainSensorByName("Bluetooth", macAddressC, baudRate);
    if (obtainErrorC) {
        std::cout << "Cannot connect to sensor C" << std::endl;
        client.close();
        return obtainErrorC;
    }
    auto imuC = sensorC.getAnyComponentOfType(g_zenSensorType_Imu);
    if (!imuC) {
        std::cout << "Sensor C has no IMU" << std::endl;
        client.close();
        return ZenError_WrongSensorType;
    }

    // --- Configure all IMUs ---
    for (auto imu : { *imuA, *imuB, *imuC }) {
        auto error = imu.setInt32Property(ZenImuProperty_SamplingRate, samplingRate);
        if (error) {
            std::cout << "Error setting sampling rate" << std::endl;
            client.close();
            return error;
        }
        auto freqPair = imu.getInt32Property(ZenImuProperty_SamplingRate);
        if (freqPair.first) {
            std::cout << "Error fetching sampling rate" << std::endl;
            client.close();
            return freqPair.first;
        }
        std::cout << "Streaming frequency: " << freqPair.second << " Hz" << std::endl;

        error = imu.setBoolProperty(ZenImuProperty_OutputLinearAcc, true);
        if (error) {
            std::cout << "Error toggling linear acc output" << std::endl;
            client.close();
            return error;
        }
    }

    // --- Synchronize sensors ---
    imuA->executeProperty(ZenImuProperty_StartSensorSync);
    imuB->executeProperty(ZenImuProperty_StartSensorSync);
    imuC->executeProperty(ZenImuProperty_StartSensorSync);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    imuA->executeProperty(ZenImuProperty_StopSensorSync);
    imuB->executeProperty(ZenImuProperty_StopSensorSync);
    imuC->executeProperty(ZenImuProperty_StopSensorSync);
    // ---------------------------

    // Raw sensor handles used for event matching
    auto sensorAHandle = sensorA.sensor();
    auto sensorBHandle = sensorB.sensor();
    auto sensorCHandle = sensorC.sensor();

    // IMU component handles used for event filtering
    auto imuAHandle = imuA->component().handle;
    auto imuBHandle = imuB->component().handle;
    auto imuCHandle = imuC->component().handle;

    // ------------------------------------------------------------------
    // Per-sensor timing instrumentation
    // ------------------------------------------------------------------
    using Clock = std::chrono::steady_clock;
    struct SensorTiming {
        std::vector<double> intervalsMs;
        Clock::time_point prevEventTime{};
        Clock::time_point firstEventTime{};
        bool havePrev = false;
        int samples = 0;
    };
    SensorTiming timingA, timingB, timingC;
    timingA.intervalsMs.reserve(128);
    timingB.intervalsMs.reserve(128);
    timingC.intervalsMs.reserve(128);

    const int maxSamplesPerSensor = 1000;
    int totalSamples = 0;

    // ------------------------------------------------------------------
    // Event loop: run until all sensors have delivered their samples
    // ------------------------------------------------------------------
    while (timingA.samples < maxSamplesPerSensor ||
           timingB.samples < maxSamplesPerSensor ||
           timingC.samples < maxSamplesPerSensor) {
        auto event = client.waitForNextEvent();
        if (!event) break;

        // Identify which sensor produced this event
        SensorTiming* timing = nullptr;
        const char* label = nullptr;

        if (sensorAHandle == event->sensor) {
            label = "A";
            timing = &timingA;
        } else if (sensorBHandle == event->sensor) {
            label = "B";
            timing = &timingB;
        } else if (sensorCHandle == event->sensor) {
            label = "C";
            timing = &timingC;
        } else {
            continue; // event from an unknown sensor
        }

        // Skip if this sensor already produced enough samples
        if (timing->samples >= maxSamplesPerSensor) continue;

        // Only process IMU component events
        if (event->component.handle != imuAHandle &&
            event->component.handle != imuBHandle &&
            event->component.handle != imuCHandle) {
            continue;
        }
        const ZenImuData& imuData = event->data.imuData;

        // Timing instrumentation
        auto now = Clock::now();
        if (!timing->havePrev) {
            timing->firstEventTime = now;
            timing->havePrev = true;
        } else {
            double dtMs = std::chrono::duration<double, std::milli>(
                now - timing->prevEventTime).count();
            timing->intervalsMs.push_back(dtMs);
        }
        timing->prevEventTime = now;

        if (verbose) {
            std::cout << ">>>>>>>>>>> Sensor " << label << " <<<<<<<<<<<\n";
            std::cout << "> Timestamp [s] \t\t " << imuData.timestamp << std::endl;
            std::cout << "> Acceleration [g] \t\t x = " << imuData.a[0]
                    << "\t y = " << imuData.a[1]
                    << "\t z = " << imuData.a[2] << std::endl;

            std::cout << "> Gyro 1 [deg/s] \t\t x = " << imuData.g1[0]
                    << "\t y = " << imuData.g1[1]
                    << "\t z = " << imuData.g1[2] << std::endl;

            std::cout << "> Magnetometer [uT] \t\t x = " << imuData.b[0]
                    << "\t y = " << imuData.b[1]
                    << "\t z = " << imuData.b[2] << std::endl;

            std::cout << "> Euler [deg] \t\t\t x = " << imuData.r[0]
                    << "\t y = " << imuData.r[1]
                    << "\t z = " << imuData.r[2] << std::endl;

            std::cout << "> Quaternions \t\t\t q0 = " << imuData.q[0]
                    << "\t q1 = " << imuData.q[1]
                    << "\t q2 = " << imuData.q[2]
                    << "\t q3 = " << imuData.q[3] << std::endl;
        } else {
            std::cout << label << std::flush;
        }

        ++timing->samples;
        ++totalSamples;
    }

    if (!verbose) std::cout << std::endl;

    // ------------------------------------------------------------------
    // Timing summary for each sensor
    // ------------------------------------------------------------------
    auto printSummary = [&](const char* name, const SensorTiming& t) {
        if (t.samples <= 1 || t.intervalsMs.empty()) {
            std::cout << "\n--- Sensor " << name << ": not enough samples ---\n";
            return;
        }
        double totalMs = std::chrono::duration<double, std::milli>(
            t.prevEventTime - t.firstEventTime).count();
        double minMs = *std::min_element(t.intervalsMs.begin(), t.intervalsMs.end());
        double maxMs = *std::max_element(t.intervalsMs.begin(), t.intervalsMs.end());
        double sumMs = std::accumulate(t.intervalsMs.begin(), t.intervalsMs.end(), 0.0);
        double meanMs = sumMs / static_cast<double>(t.intervalsMs.size());
        double observedHz = (t.samples - 1) * 1000.0 / totalMs;

        double sq = 0.0;
        for (double v : t.intervalsMs) {
            double d = v - meanMs;
            sq += d * d;
        }
        double rmsJitterMs = std::sqrt(sq / static_cast<double>(t.intervalsMs.size()));

        std::cout << "\n--- Timing summary Sensor " << name << " (wall clock) ---\n";
        std::cout << "Samples:               " << t.samples << "\n";
        std::cout << "Elapsed:               " << totalMs << " ms\n";
        std::cout << "Observed rate:         " << observedHz << " Hz"
                  << "  (requested " << samplingRate << " Hz)\n";
        std::cout << "Mean interval:         " << meanMs << " ms  ("
                  << (meanMs > 0.0 ? 1000.0 / meanMs : 0.0) << " Hz)\n";
        std::cout << "Min / Max interval:    " << minMs << " / " << maxMs << " ms\n";
        std::cout << "RMS jitter:            " << rmsJitterMs << " ms\n";
    };

    printSummary("A", timingA);
    printSummary("B", timingB);
    printSummary("C", timingC);

    client.close();
    std::cout << "\nSensor connections closed\n";
    std::cout << "Received " << totalSamples << " IMU samples total "
              << "(A=" << timingA.samples
              << ", B=" << timingB.samples
              << ", C=" << timingC.samples << ")\n";
    return 0;
}