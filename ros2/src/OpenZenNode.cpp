//===========================================================================//
//
// Copyright (C) 2021 LP-Research Inc.
//
// This file is part of OpenZenRos driver, under the MIT License.
// See the LICENSE file in the top-most folder for details
// SPDX-License-Identifier: MIT
//
//===========================================================================//

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "sensor_msgs/msg/magnetic_field.hpp"
#include "sensor_msgs/msg/nav_sat_fix.hpp"
#include "sensor_msgs/msg/nav_sat_status.hpp"

#include "std_msgs/msg/bool.hpp"

#include "std_srvs/srv/set_bool.hpp"
#include "std_srvs/srv/trigger.hpp"

#include "ManagedThread.h"
#include <OpenZen.h>

#include <memory>
#include <string>
#include <iostream>
#include <array>

class OpenZenSensor : public rclcpp::Node
{
public:
    // Parameters
    std::string m_sensorName;
    std::string m_sensorInterface;
    std::string frame_id;
    std::string frame_id_gnss;
    int m_baudrate = 0;
    bool m_configureGnssOutput = true;
    bool m_isIg1 { false };

    // IG1 sensor specific parameters
    static constexpr std::array<double, 9> IG1_ANGULAR_VELOCITY_COVARIANCE = {
        1.7175e-06, 0.0, 0.0,
        0.0, 1.6753e-06, 0.0,
        0.0, 0.0, 1.6094e-06
    };

    OpenZenSensor() : Node("openzen_node"),
            m_sensorThread( [&](SensorThreadParams const& param) -> bool {

            const float cDegToRad = 3.1415926f/180.0f;
            const float cEarthG = 9.81f;
            const float cMicroToTelsa = 1e-6f;

            const auto event = param.zenClient->waitForNextEvent();

            if (!event.has_value())
            {
                // empty event received, terminate
                return false;
            }

            if (!event->component.handle)
            {
                // not an event from a component
                switch (event->eventType)
                {
                    case ZenEventType_SensorDisconnected:
                        RCLCPP_INFO(get_logger(), "OpenZen sensor disconnected");
                        return false;
                }
            }

            if (event->component == param.zen_imu_component)
            {
                if (event->eventType == ZenEventType_ImuData)
                {
                    // IMU
                    auto const& d = event->data.imuData;

                    sensor_msgs::msg::Imu imu_msg;
                    sensor_msgs::msg::MagneticField mag_msg;

                    // We follow this ROS conventions
                    // https://www.ros.org/reps/rep-0103.html
                    // https://www.ros.org/reps/rep-0145.html

                    imu_msg.header.stamp = this->now();
                    imu_msg.header.frame_id = param.frame_id;

                    // Fill orientation quaternion
                    imu_msg.orientation.w = d.q[0];
                    imu_msg.orientation.x = -d.q[1];
                    imu_msg.orientation.y = -d.q[2];
                    imu_msg.orientation.z = -d.q[3];

                    // Fill angular velocity data
                    // - scale from deg/s to rad/s
                    // - please check OpenZen docs for difference between gyro1 (high precision) and gyro2 (general purpose)
                    // https://lp-research.atlassian.net/wiki/spaces/LKB/pages/2005630977/OpenZen+Documentations#Keys-for-Sensor-Data-Access
                    imu_msg.angular_velocity.x = d.g2[0] * cDegToRad;
                    imu_msg.angular_velocity.y = d.g2[1] * cDegToRad;
                    imu_msg.angular_velocity.z = d.g2[2] * cDegToRad;

                    // Set angular velocity covariance for IG1 sensors
                    if (m_isIg1) {
                        std::copy(IG1_ANGULAR_VELOCITY_COVARIANCE.begin(), 
                                IG1_ANGULAR_VELOCITY_COVARIANCE.end(), 
                                imu_msg.angular_velocity_covariance.begin());
                    }

                    // Fill linear acceleration data
                    const float rosConversion = -1.0 * (!param.useLpmsAccelerationConvention) +
                        1.0 * param.useLpmsAccelerationConvention;

                    imu_msg.linear_acceleration.x = rosConversion * d.a[0] * cEarthG;
                    imu_msg.linear_acceleration.y = rosConversion * d.a[1] * cEarthG;
                    imu_msg.linear_acceleration.z = rosConversion * d.a[2] * cEarthG;

                    mag_msg.header.stamp = imu_msg.header.stamp;
                    mag_msg.header.frame_id = param.frame_id;

                    // Units are microTesla in the LPMS library, Tesla in ROS.
                    mag_msg.magnetic_field.x = d.b[0] * cMicroToTelsa;
                    mag_msg.magnetic_field.y = d.b[1] * cMicroToTelsa;
                    mag_msg.magnetic_field.z = d.b[2] * cMicroToTelsa;

                    // Publish the messages
                    param.imu_pub->publish(imu_msg);
                    param.mag_pub->publish(mag_msg);
                }
            } else if (event->component == param.zen_gnss_component) {
                if (event->eventType == ZenEventType_GnssData) {
                    // Global navigation satellite system
                    auto const& d = event->data.gnssData;
                    sensor_msgs::msg::NavSatFix nav_msg;
                    sensor_msgs::msg::NavSatStatus nav_status;
                    nav_status.status = sensor_msgs::msg::NavSatStatus::STATUS_NO_FIX;

                    if (d.fixType == ZenGnssFixType_2dFix ||
                        d.fixType == ZenGnssFixType_3dFix ||
                        d.fixType == ZenGnssFixType_GnssAndDeadReckoning){
                            nav_status.status = sensor_msgs::msg::NavSatStatus::STATUS_FIX;
                        }

                    // even better, do we have an RTK navigation solution ?
                    if (d.carrierPhaseSolution == ZenGnssFixCarrierPhaseSolution_FloatAmbiguities ||
                        d.carrierPhaseSolution == ZenGnssFixCarrierPhaseSolution_FixedAmbiguities) {
                            nav_status.status = sensor_msgs::msg::NavSatStatus::STATUS_GBAS_FIX;
                    }

                    // OpenZen does not output the exact satellite service so assume its
                    // only GPS for now
                    nav_status.service = sensor_msgs::msg::NavSatStatus::SERVICE_GPS;

                    nav_msg.status = nav_status;
                    nav_msg.latitude = d.latitude;
                    nav_msg.longitude = d.longitude;
                    nav_msg.altitude = d.height;

                    // initialize all members to zero
                    nav_msg.position_covariance = {0};
                    // OpenZen provides accuracy at 1-sigma in meters
                    // here we need covariance entries with m^2
                    nav_msg.position_covariance[0] = std::pow(d.horizontalAccuracy, 2);
                    nav_msg.position_covariance[4] = std::pow(d.verticalAccuracy, 2);
                    // OpenZen GNNS Sensor does not provide an height estimation. Assume a
                    // conservative height estimation of 10 meters accuracy.
                    nav_msg.position_covariance[8] = std::pow(10.0, 2);

                    nav_msg.position_covariance_type = nav_msg.COVARIANCE_TYPE_APPROXIMATED;

                    nav_msg.header.stamp = this->now();
                    nav_msg.header.frame_id = param.frame_id_gnss;

                    param.nav_pub->publish(nav_msg);
                }
            }
                
            return true;
        }) {

        m_imu_pub  = this->create_publisher<sensor_msgs::msg::Imu>("data", 10);
        m_mag_pub  = this->create_publisher<sensor_msgs::msg::MagneticField>("mag", 10);
        m_autocalibration_status_pub = this->create_publisher<std_msgs::msg::Bool>("is_autocalibration_active", 10);

        // Get node parameters
        m_sensorName = this->declare_parameter<std::string>("sensor_name", "");
        m_sensorInterface = this->declare_parameter<std::string>("sensor_interface", "LinuxDevice");
        m_openzenVerbose = this->declare_parameter<bool>("openzen_verbose", false);
        
        // Using 0 as default will tell OpenZen to use the defaul baudrate for a respective sensor
        m_baudrate = this->declare_parameter<int>("baudrate", 0);
        m_configureGnssOutput = this->declare_parameter<int>("configure_gnss_output", true);

        // In LP-Research sensor output, the linear acceleration measurement is pointing down (z-) when
        // the sensor is lying flat on the table. ROS convention is z+ pointing up in this case
        // By default, this ROS driver converts to the ROS convention. Set this flag to true to
        // use the LPMS convention
        m_useLpmsAccelerationConvention = this->declare_parameter<bool>("use_lpms_acceleration_convention", false);
        frame_id = this->declare_parameter<std::string>("frame_id", "imu");
        frame_id_gnss = this->declare_parameter<std::string>("frame_id_gnss", "gnss");

        m_autocalibration_serv = this->create_service<std_srvs::srv::SetBool>
            ("enable_gyro_autocalibration",
            std::bind(&OpenZenSensor::handleService_setAutocalibration, this, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3));

        m_resetheading_serv = this->create_service<std_srvs::srv::Trigger>
            ("reset_heading",
            std::bind(&OpenZenSensor::handleService_resetHeading, this, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3));

        m_gyrocalibration_serv = this->create_service<std_srvs::srv::Trigger>
            ("calibrate_gyroscope",
            std::bind(&OpenZenSensor::handleService_calibrateGyroscope, this, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3));

        auto clientPair = zen::make_client();
        m_zenClient = std::unique_ptr<zen::ZenClient>(new zen::ZenClient(std::move(clientPair.second)));

        if (clientPair.first != ZenError_None)
        {
            RCLCPP_ERROR(get_logger(),"Cannot start OpenZen");
            return;
        }

        if (m_openzenVerbose)
        {
            ZenSetLogLevel(ZenLogLevel_Debug);
        } 
        else
        {
            ZenSetLogLevel(ZenLogLevel_Off);
        }

        // No sensor name given, auto-discovery
        if (m_sensorName.size() == 0) 
        {
            RCLCPP_INFO(get_logger(),"OpenZen sensors will be listed");
            ZenError listError = m_zenClient->listSensorsAsync();

            if (listError != ZenError_None)
            {
                RCLCPP_ERROR(get_logger(),"Cannot list sensors");
                return;
            }
            
            bool listingDone = false;
            bool firstSensorFound = false;
            ZenSensorDesc foundSens;

            while (listingDone == false)
            {
                const auto event = m_zenClient->waitForNextEvent();
                if (!event.has_value())
                    break;

                if (!event->component.handle)
                {
                    switch (event->eventType)
                    {
                    case ZenEventType_SensorFound:
                        if (!firstSensorFound)
                        {
                            foundSens = event->data.sensorFound;
                            firstSensorFound = true;
                        }
                        RCLCPP_INFO_STREAM(get_logger(),"OpenZen sensor with name " << event->data.sensorFound.serialNumber << " on IO system " <<
                            event->data.sensorFound.ioType << " found");
                        break;

                    case ZenEventType_SensorListingProgress:
                        if (event->data.sensorListingProgress.progress == 1.0f)
                        {
                            listingDone = true;
                        }
                            
                        break;
                    }
                }
            }

            if (!firstSensorFound)
            {
                RCLCPP_ERROR(get_logger(),"No OpenZen sensors found");
                return;
            }

            RCLCPP_INFO_STREAM(get_logger(), "Connecting to found sensor " << foundSens.serialNumber << " on IO system " << foundSens.ioType);
            
            // If a baudRate has been set, override the default given by OpenZen listing
            if (m_baudrate > 0) {
                foundSens.baudRate = m_baudrate;
            }

            auto sensorObtainPair = m_zenClient->obtainSensor(foundSens);

            if (sensorObtainPair.first != ZenSensorInitError_None)
            {
                RCLCPP_ERROR(get_logger(),"Cannot connect to sensor found with discovery. Make sure you have the user rights to access serial devices.");
                return;
            }
            m_zenSensor = std::unique_ptr<zen::ZenSensor>( new zen::ZenSensor(std::move(sensorObtainPair.second)));
        } 
        else
        {
            // Directly connect to sensor
            RCLCPP_INFO_STREAM(get_logger(), "Connecting directly to sensor " << m_sensorName << " over interface " << m_sensorInterface);
            // auto sensorObtainPair = m_zenClient->obtainSensorByName(m_sensorInterface, m_sensorName, m_baudrate);
            auto sensorObtainPair = m_zenClient->obtainSensorByName(
                "Bluetooth",
                "00:04:3E:53:ED:5B",
                921600);            

            if (sensorObtainPair.first != ZenSensorInitError_None)
            {
                RCLCPP_ERROR(get_logger(), "Cannot connect directly to sensor.  Make sure you have the user rights to access serial devices.");
                return;
            }
            m_zenSensor = std::unique_ptr<zen::ZenSensor>( new zen::ZenSensor(std::move(sensorObtainPair.second)));
        }

        if (!startStreaming()) {
            RCLCPP_ERROR(get_logger(), "Cannot start sensor data streaming");
        } else {
            const auto& [e, id] = m_zenSensor->getStringProperty(ZenSensorProperty_SensorModel);
            RCLCPP_INFO_STREAM(get_logger(), "Sensor model ID: " << id);

            std::string idLC;
            std::transform(std::begin(id), std::end(id), std::back_inserter(idLC),
                [](auto c) { return (char)std::tolower(c); });

            m_isIg1 = idLC.find("ig1", 0) != std::string::npos;

            if (m_isIg1) {
                RCLCPP_INFO_STREAM(get_logger(), "IG1 sensor detected, using the appropriate angular velocity covariance matrix");
            }
        }
    }

  bool startStreaming() {
        if (!m_zenClient)
        {
            RCLCPP_ERROR(get_logger(), "OpenZen could not be started");
            return false;
        }

        if (!m_zenSensor)
        {
            RCLCPP_ERROR(get_logger(), "OpenZen sensor could not be connected");
            return false;
        }

        ZenComponentHandle_t zen_imu_component = {0};
        ZenComponentHandle_t zen_gnss_component = {0};

        if (m_sensorInterface == "TestSensor") {
            // Test sensor does not return any components when queried but still
            // provides IMU measurement with component 1
            zen_imu_component.handle = 1;
        }

        const auto lmdEnableOutput = [this](auto & component, auto property) {
            if (component->setBoolProperty(property, true) != ZenError_None) {
                RCLCPP_ERROR_STREAM(get_logger(), "Cannot enable output of value " << property << " on sensor");
            }
        };

        auto imu_component = m_zenSensor->getAnyComponentOfType(g_zenSensorType_Imu);
        if (!imu_component.has_value())
        {
            // error, this sensor does not have an IMU component
            RCLCPP_INFO(get_logger(),"No IMU component available, sensor control commands won't be available");
        } else {
            RCLCPP_INFO(get_logger(),"IMU component found");
            m_zenImu = std::unique_ptr<zen::ZenSensorComponent>( new zen::ZenSensorComponent(std::move(imu_component.value())));
            zen_imu_component = m_zenImu->component();

            // Automatic output flag settings for IMU not done atm because this
            // depends on the sensor model (legacy protocol or IG1 which output fields)
            // need to be enable and this is not exposed by OpenZen at the moment

            publishIsAutocalibrationActive();
        }

        auto gnss_component = m_zenSensor->getAnyComponentOfType(g_zenSensorType_Gnss);
        if (!gnss_component.has_value())
        {
            // Error, this sensor does not have an IMU component
            RCLCPP_INFO(get_logger(), "No GNSS component available, sensor won't provide Global positioning data");
        } else {
            RCLCPP_INFO(get_logger(), "GNSS component found");
            m_zenGnss = std::unique_ptr<zen::ZenSensorComponent>( new zen::ZenSensorComponent(std::move(gnss_component.value())));
            zen_gnss_component = m_zenGnss->component();

            if (m_configureGnssOutput) {
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvtFixType);

                // Used to read the RTK GPS state
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvtFlags);
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvtNumSV);
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvtLongitude);
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvtLatitude);
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvtHeight);
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvthMSL);
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvthAcc);
                lmdEnableOutput(m_zenGnss, ZenGnssProperty_OutputNavPvtvAcc);
            }

            // set up a publisher for Gnss
            m_nav_pub = this->create_publisher<sensor_msgs::msg::NavSatFix>("nav",1);
        }

        m_sensorThread.start( SensorThreadParams {
            m_zenClient.get(),
            frame_id,
            frame_id_gnss,
            m_imu_pub,
            m_mag_pub,
            m_nav_pub,
            m_useLpmsAccelerationConvention,
            zen_imu_component,
            zen_gnss_component
        } );

        RCLCPP_INFO(get_logger(), "Data streaming from sensor started");

        return true;    
  }

    // Service callbacks
    void publishIsAutocalibrationActive()
    {
        std_msgs::msg::Bool msg;

        if (!m_zenImu) {
            RCLCPP_INFO(get_logger(), "No IMU compontent available, can't publish autocalibration status");
            return;
        }

        auto resPair = m_zenImu->getBoolProperty(ZenImuProperty_GyrUseAutoCalibration);
        auto error = resPair.first;
        auto useAutoCalibration = resPair.second;
        if (error) 
        {
            RCLCPP_ERROR(get_logger(),"Cannot load autocalibration status from sensor");
        }
        else 
        {
            msg.data = useAutoCalibration;
            m_autocalibration_status_pub->publish(msg);   
        }
    }

    bool handleService_setAutocalibration (
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<std_srvs::srv::SetBool::Request> req,
        std::shared_ptr<std_srvs::srv::SetBool::Response> res)
    {
        RCLCPP_INFO(get_logger(),"set_autocalibration service triggered");

        std::string msg;

        if (!m_zenImu) {
            RCLCPP_INFO(get_logger(),"No IMU compontent available, can't set autocalibration status");
            return false;
        }

        if (auto error = m_zenImu->setBoolProperty(ZenImuProperty_GyrUseAutoCalibration, req->data))
        {
            RCLCPP_ERROR(get_logger(),"Error while setting auto-calibration");
            res->success = false; 
            msg.append(std::string("[Failed] current autocalibration status set to: ") + (req->data?"True":"False"));
        
        }
        else
        {
            res->success = true;
            msg.append(std::string("[Success] autocalibration status set to: ") + (req->data?"True":"False"));
        }

        publishIsAutocalibrationActive();        
        res->message = msg;

        return res->success;
    }

    bool handleService_resetHeading (
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<std_srvs::srv::Trigger::Request> req,
        std::shared_ptr<std_srvs::srv::Trigger::Response> res)
    {
        if (!m_zenImu) {
            RCLCPP_INFO(get_logger(),"No IMU compontent available, can't reset heading");
            return false;
        }

        RCLCPP_INFO(get_logger(),"Resetting heading");
        
        // Offset reset parameters:
        // 0: Object reset
        // 1: Heading reset
        // 2: Alignment reset
        if (auto error = m_zenImu->setInt32Property( ZenImuProperty_OrientationOffsetMode, 1)) 
        {
            RCLCPP_ERROR(get_logger(),"Error while resetting heading");
            res->success = false;
            res->message = "[Failed] Heading reset";
        } 
        else 
        {
            res->success = true;
            res->message = "[Success] Heading reset";
        }

        return res->success;
    }

    bool handleService_calibrateGyroscope (
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<std_srvs::srv::Trigger::Request> req,
        std::shared_ptr<std_srvs::srv::Trigger::Response> res)
    {
        if (!m_zenImu) {
            RCLCPP_ERROR(get_logger(),"No IMU compontent available, can't start autocalibration");
            return false;
        }

        RCLCPP_INFO(get_logger(),"calibrate_gyroscope: Please make sure the sensor is stationary for 4 seconds");

        if (auto error = m_zenImu->executeProperty(ZenImuProperty_CalibrateGyro))
        {
            RCLCPP_ERROR(get_logger(),"Error while starting autocalibration");

            res->success = false;
            res->message = "[Failed] Gyroscope calibration procedure error";
        }
        else
        {
            rclcpp::sleep_for(std::chrono::seconds(4));
            res->success = true;
            res->message = "[Success] Gyroscope calibration procedure completed";
            RCLCPP_INFO(get_logger(),"calibrate_gyroscope: Gyroscope calibration procedure completed");

        }
        return res->success;
    }

 private:

    std::unique_ptr<zen::ZenClient> m_zenClient;
    std::unique_ptr<zen::ZenSensor> m_zenSensor;
    std::unique_ptr<zen::ZenSensorComponent> m_zenImu;

    // Might be null if no Gnss component is available
    std::unique_ptr<zen::ZenSensorComponent> m_zenGnss;

    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr m_autocalibration_serv;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_resetheading_serv;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_gyrocalibration_serv;

    // ROS2 publishers
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr m_imu_pub;
    rclcpp::Publisher<sensor_msgs::msg::MagneticField>::SharedPtr m_mag_pub;
    rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr m_nav_pub;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr m_autocalibration_status_pub;

    bool m_openzenVerbose;
    bool m_useLpmsAccelerationConvention;

    struct SensorThreadParams
    {
        zen::ZenClient * zenClient;
        std::string frame_id;
        std::string frame_id_gnss;
        rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub;
        rclcpp::Publisher<sensor_msgs::msg::MagneticField>::SharedPtr mag_pub;
        rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr & nav_pub;
        bool useLpmsAccelerationConvention;
        ZenComponentHandle_t zen_imu_component;
        ZenComponentHandle_t zen_gnss_component;
    };

    ManagedThread<SensorThreadParams> m_sensorThread;    
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<OpenZenSensor>();
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}
