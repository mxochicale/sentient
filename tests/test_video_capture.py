"""Test video capture"""

import glob

from sentient.video_devices.capturing_video import capture_video


def test_simple_list_of_available_video_devices():
    """
    To check which devices are available.
    TODO. use list ids for different tests
    """
    ids = []
    for camera in glob.glob("/dev/video?"):
        ids.append(camera)

    print(ids)
    assert len(ids) > 0


def test_capture_video():
    """
    Testing captured frames
    """
    vfn = "video_capturevideotest.mp4"
    id_framegrabber = 0
    fwidth = 480
    fheigth = 640
    frame_per_second = 30  # 120
    buffer_size = 1

    last_captured_frame = capture_video(
        vfn, id_framegrabber, fwidth, fheigth, frame_per_second, buffer_size
    )
    print(f"  Test: Shape of last capture frame {last_captured_frame.shape}")
    assert last_captured_frame.shape[1] > 0
