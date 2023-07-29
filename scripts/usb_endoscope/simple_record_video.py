# https://www.learningaboutelectronics.com/Articles/How-to-record-video-Python-OpenCV.php
import argparse
import cv2


def decode_fourcc(cc):
    return "".join([chr((int(cc) >> 8 * i) & 0xFF) for i in range(4)])


def SimpleCaptureVideo(video_filename, idFG):
    cap = cv2.VideoCapture(idFG)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    # fourcc = cv2.VideoWriter_fourcc(*'DIVX')
    # fourcc = cv2.VideoWriter_fourcc(*'MJPG')
    # fourcc = cv2.VideoWriter_fourcc(*'YUYV')
    print(cap.get)
    getting_fcc = int(cap.get(cv2.CAP_PROP_FOURCC))
    print(f'getting_fcc: {getting_fcc}')
    print(f'decode_fourcc: {decode_fourcc(getting_fcc)}')

    out_video = cv2.VideoWriter(video_filename, fourcc, 20, (width, height))

    while True:
        ret, frame = cap.read()

        frame = cv2.cvtColor(frame, cv2.COLOR_RGBA2RGB)
        # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # frame = cv2.cvtColor(frame, cv2.COLOR_YUV2RGB)
        # frame = cv2.cvtColor(frame, cv2.COLOR_YUV2RGB_NV12) # Invalid number of channels in input image:
        # frame = cv2.cvtColor(frame, cv2.COLOR_YUV2RGB_UYVY) # Invalid number of channels in input image:

        out_video.write(frame)

        if frame is not None:
            cv2.imshow('frame', frame)
        else:
            print('No frame')

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    out_video.release()

    cv2.destroyAllWindows()

    return frame

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--vfn', required=None, help='Specify video file name', type=str)
    parser.add_argument('--idFG', required=True, help='Specify ID for framegrabber', type=int)
    args = parser.parse_args()

    SimpleCaptureVideo(args.vfn, args.idFG)
