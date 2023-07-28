from scripts.usb_endoscope.capturing_video import CaptureVideoTest
from scripts.usb_endoscope.simple_record_video import SimpleCaptureVideo


def test_capture_frames():
    """
    Testing captured frames
    """
    vfn = 'video_capturevideotest.mp4'
    idFG = 4
    fW = 480
    fH = 640
    FPS = 120
    buffer_size = 1

    last_captured_frame = CaptureVideoTest(vfn, idFG, fW, fH, FPS, buffer_size)
    assert (last_captured_frame.shape[2] > 0 )


def test_simple_recording_video():
    """
    Testing simple_record_video
    """
    idFG = 4
    vfn = 'video_simpleCaptured.mp4'
    last_captured_frame = SimpleCaptureVideo(vfn, idFG)
    assert (last_captured_frame.shape[2] > 0)
