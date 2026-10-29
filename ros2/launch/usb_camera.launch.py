from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='openzen_driver',
            executable='openzen_node',
            namespace='openzen',
            name='openzen_node',
            output='screen'
        ),
        Node(
            package='v4l2_camera',
            executable='v4l2_camera_node',
            namespace='camera',
            name='usb_cam',
            parameters=[{
                'video_device': '/dev/video0',
                'image_size': [320, 240],
                'pixel_format': 'YUYV',
                'camera_frame_id': 'usb_cam_link'
            }],
            output='screen'
        )
    ])
