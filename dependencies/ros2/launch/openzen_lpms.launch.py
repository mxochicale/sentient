from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='openzen_driver',
            namespace='openzen',
            executable='openzen_node',
            name='lpms_node'
        ),

        # Note: topic tools not ported to ROS2 yet, so no easy conversion
        #       from quaternion to euler available (https://github.com/ros2/ros2/issues/857)

        Node(
            package="rqt_plot",
            executable="rqt_plot",
            name="ig1_data_plotter",
            namespace="openzen",
            arguments=["/openzen/data/linear_acceleration/x:y:z"]
        )
    ])
