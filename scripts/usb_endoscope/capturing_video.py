import argparse
import cv2

def decode_fourcc(cc):
    return "".join([chr((int(cc) >> 8 * i) & 0xFF) for i in range(4)])


def CaptureVideoTest(video_filename, id_framegrabber, frame_width, frame_height, frames_per_second, buffer_size):
    cap = cv2.VideoCapture(id_framegrabber, cv2.CAP_V4L2)
    cap.set(cv2.CAP_PROP_BUFFERSIZE, buffer_size)
    # cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('Y', 'U', 'Y', 'V'))
    # cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('B', 'G', 'R', '3')) #don't make any effect and leave it as "YUYV"
    # cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('X', 'V', 'I', 'D')) #don't make any effect and leave it as "YUYV"
    # cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('M', 'J', 'P', 'G')) #  motion-jpeg codec
    # cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'DIVX'))
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
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
    # fcc = int(cap.get(cv2.CAP_PROP_FOURCC))
    bs = cap.get(cv2.CAP_PROP_BUFFERSIZE)

    print('    ')
    print('  Video properties:')
    print('     fps: {}'.format(fps))
    print('     resolution: {}Wx{}H'.format(w, h))
    # print(f'      fcc: {fcc}')
    # print(f'      decode_fourcc: {decode_fourcc(fcc)}')
    print('     buffer size: {}'.format(bs))
    #  writer= cv2.VideoWriter('basicvideo.mp4', fourcc, 20, (w,h))
    out_video= cv2.VideoWriter(video_filename, fourcc, frames_per_second, (w,h))

    while (True):
        ret, frame = cap.read()
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
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

    CaptureVideoTest(args.vfn, args.idFG, args.fW, args.fH, args.FPS, args.buffer_size)

if __name__ == '__main__':
    main()
