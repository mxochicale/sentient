from rtt4ssa.usb_endoscope.capturing_video import CaptureVideoTest
from rtt4ssa.usb_endoscope.simple_record_video import SimpleCaptureVideo

import cv2, glob

def test_simple_list_of_available_video_devices():
    """
    To check which devices are available.
    TODO. use list ids for different tests
    """
    ids = []
    for camera in glob.glob("/dev/video?"):
        ids.append(camera)

    print(ids)
    assert len(ids)>0


def test_simple_recording_video():
    """
    Testing simple_record_video
    """
    idFG = 0
    vfn = 'video_simpleCaptured.mp4'
    last_captured_frame = SimpleCaptureVideo(vfn, idFG)
    assert (last_captured_frame.shape[2] > 0)

def test_capture_video():
    """
    Testing captured frames
    """
    vfn = 'video_capturevideotest.mp4'
    idFG = 0
    fW = 480
    fH = 640
    FPS = 120
    buffer_size = 1

    last_captured_frame = CaptureVideoTest(vfn, idFG, fW, fH, FPS, buffer_size)
    #print(last_captured_frame.shape)
    assert (last_captured_frame.shape[1] > 0 )
