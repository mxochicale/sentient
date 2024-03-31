import argparse
import cv2

def decode_fourcc(cc):
    return "".join([chr((int(cc) >> 8 * i) & 0xFF) for i in range(4)])


def CaptureVideo(video_filename, id_framegrabber, frame_width, frame_height, frames_per_second, buffer_size):
    cap = cv2.VideoCapture(id_framegrabber, cv2.CAP_V4L2)
    cap.set(cv2.CAP_PROP_BUFFERSIZE, buffer_size)
    #fourcc = cv2.VideoWriter_fourcc(*'XVID') ##decoded as YUYV
    #fourcc = cv2.VideoWriter_fourcc('X', 'V', 'I', 'D')
    #fourcc = cv2.VideoWriter_fourcc(*'DIVX') #decoded as YUYV
    fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v') #decoded as YUYV
    #fourcc = cv2.VideoWriter_fourcc('M', 'J', 'P', 'G') #ffmeg -i yuvj420p(pc, bt470bg/un/un)
    #fourcc = cv2.VideoWriter_fourcc(*'MJPG')
    #fourcc = cv2.VideoWriter_fourcc('Y', 'U', 'Y', 'V') #codec not currently supported in container
    #fourcc = cv2.VideoWriter_fourcc(*'YUYV') #codec not currently supported in container
    #fourcc = cv2.VideoWriter_fourcc('a', 'v', 'c', '1') #Encoder not found
    #fourcc = cv2.VideoWriter_fourcc('H','2','6','4') #Encoder not found
    #fourcc = cv2.VideoWriter_fourcc('I','4','2','0') #codec not currently supported in container
    #fourcc = cv2.VideoWriter_fourcc('B', 'G', 'R', '3') #don't work
    cap.set(cv2.CAP_PROP_FOURCC, fourcc)
    cap.set(cv2.CAP_PROP_FPS, frames_per_second)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, frame_width)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frame_height)
 
    # Check if the device is opened correctly
    if not cap.isOpened():
        raise IOError("Cannot open video source {}".format(id_framegrabber))

    # print properties of te capture
    fps = cap.get(cv2.CAP_PROP_FPS)
    w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fcc = int(cap.get(cv2.CAP_PROP_FOURCC))
    bs = cap.get(cv2.CAP_PROP_BUFFERSIZE)

    print(f'    ')
    print(f'  Video properties:')
    print(f'     fps: {fps}')
    print(f'     resolution: W{w}xH{h}')
    print(f'     fcc: {fcc}')
    print(f'     decode_fourcc: {decode_fourcc(fcc)}')
    print(f'     buffer size: {bs}')
    out_video= cv2.VideoWriter(video_filename, fourcc, frames_per_second, (w,h))

    while (True):
        ret, frame = cap.read()
        frame = cv2.cvtColor(frame, cv2.COLOR_RGBA2RGB)
        # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
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

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--vfn', required=None, help='Specify video file name', type=str)
    parser.add_argument('--idFG', required=True, help='Specify ID for framegrabber', type=int)
    parser.add_argument('--fW', required=True, help='Specify width of the image', type=int)
    parser.add_argument('--fH', required=True, help='Specify high of the image', type=int)
    parser.add_argument('--FPS', required=True, help='Specify FPS', type=int)
    parser.add_argument('--buffer_size', required=True, help='Specify buffer_size', type=int)
    args = parser.parse_args()

    CaptureVideo(args.vfn, args.idFG, args.fW, args.fH, args.FPS, args.buffer_size)

if __name__ == '__main__':
    main()
