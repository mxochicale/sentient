from sentient.video_devices.capturing_video import CaptureVideo

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


def test_capture_video():
    """
    Testing captured frames
    """
    vfn = 'video_capturevideotest.mp4'
    idFG = 0
    fW = 480
    fH = 640
    FPS = 30 #120
    buffer_size = 1

    last_captured_frame = CaptureVideo(vfn, idFG, fW, fH, FPS, buffer_size)
    print(last_captured_frame.shape)
    assert (last_captured_frame.shape[1] > 0 )
