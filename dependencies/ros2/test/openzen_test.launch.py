from launch import LaunchDescription
from launch_ros.actions import Node

import unittest
import time
import launch_testing
import launch_testing.actions
from launch_testing.asserts import assertSequentialStdout

#from rclpy.node import Node
#import rclpy
import rclpy
from sensor_msgs.msg import Imu

def generate_test_description():
    return LaunchDescription([
        Node(
            package='openzen_driver',
            namespace='imu',
            executable='openzen_node',
            parameters=[{
                "sensor_interface" : "TestSensor",
                "sensor_name" : "Sensor1"
            }],
            name='lpms_node'
        ),
        launch_testing.actions.ReadyToTest()
    ])


class TestGoodProcess(unittest.TestCase):

    def test_count_to_four(self, proc_output):

        received_data = False

        def incoming_imu(imu_data):
            nonlocal received_data
            received_data = True
            self.assertAlmostEqual(0.0, imu_data.linear_acceleration.x, 2)
            self.assertAlmostEqual(0.0, imu_data.linear_acceleration.y, 2)
            self.assertAlmostEqual(9.81, imu_data.linear_acceleration.z, 2)

            # the test sensor will output some arbitrary gyro values
            self.assertLess(0.0, imu_data.angular_velocity.x)
            self.assertLess(0.0, imu_data.angular_velocity.y, 2)
            self.assertLess(0.0, imu_data.angular_velocity.z, 2)

        rclpy.init()
        node = rclpy.create_node('test_openzen')
        sub = node.create_subscription(
            msg_type=Imu,
            topic='/imu/data',
            callback=incoming_imu,
            qos_profile=10)

        start_time = time.time()
        while rclpy.ok():
            rclpy.spin_once(node, timeout_sec=1)
            test_time = time.time() - start_time

            # sample 1 second
            if test_time > 1:
                break

        node.destroy_node()
        rclpy.shutdown()

        self.assertTrue(received_data,"No IMU data received from test node")

@launch_testing.post_shutdown_test()
class TestProcessOutput(unittest.TestCase):

    def test_exit_code(self, proc_info):
        # Check that all processes in the launch (in this case, there's just one) exit
        # with code 0
        launch_testing.asserts.assertExitCodes(proc_info)
