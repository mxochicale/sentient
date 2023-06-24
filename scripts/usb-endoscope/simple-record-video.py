#https://www.learningaboutelectronics.com/Articles/How-to-record-video-Python-OpenCV.php
import cv2

def decode_fourcc(cc):
    return "".join([chr((int(cc) >> 8 * i) & 0xFF) for i in range(4)])


def main():
    cap= cv2.VideoCapture(0)
    width= int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height= int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    # fourcc = cv2.VideoWriter_fourcc(*'DIVX')
    # fourcc = cv2.VideoWriter_fourcc(*'MJPG')
    # fourcc = cv2.VideoWriter_fourcc(*'YUYV')

    getting_fcc = int(cap.get(cv2.CAP_PROP_FOURCC))
    print(f'getting_fcc: {getting_fcc}')
    print(f'decode_fourcc: {decode_fourcc(getting_fcc)}')

    out_video= cv2.VideoWriter('basicvideo.mp4', fourcc, 20, (width, height))

    while True:
        ret,frame= cap.read()
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


if __name__ == '__main__':
    main()
