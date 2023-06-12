import argparse
import csv
import sys
import time
from threading import Thread
from timeit import default_timer as timer

import cv2
import numpy as np
import openzen
# import matplotlib.pyplot as plt

class VideoStreamWidget(object):
    """
    # Class for VideoStreamWidget
    https://stackoverflow.com/questions/55099413
    """

    def __init__(self, id_framegrabber=0, frames_per_second=60, frame_width=640, frame_height=480, video_filename=None):
        self.id_framegrabber = id_framegrabber
        self.capture = cv2.VideoCapture(self.id_framegrabber, cv2.CAP_V4L2)
        self.buffer_size = 1
        self.frames_per_second = frames_per_second
        self.frame_width = frame_width
        self.frame_height = frame_height
        self.video_filename = video_filename
        self.capture.set(cv2.CAP_PROP_BUFFERSIZE, self.buffer_size)
        self.fourcc = cv2.VideoWriter_fourcc('M', 'J', 'P', 'G')
        self.capture.set(cv2.CAP_PROP_FOURCC, self.fourcc)  # motion-jpeg codec
        self.capture.set(cv2.CAP_PROP_FPS, self.frames_per_second)
        self.capture.set(cv2.CAP_PROP_FRAME_WIDTH, self.frame_width)
        self.capture.set(cv2.CAP_PROP_FRAME_HEIGHT, frame_height)
        self.videowriter = cv2.VideoWriter(self.video_filename, self.fourcc, self.frames_per_second,
                                           (self.frame_width, self.frame_height), 0)
        # Start the thread to read frames from the video stream
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()

    def update(self):
        # Read the next frame from the stream in a different thread
        while True:
            if self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
                self.frame = cv2.cvtColor(self.frame, cv2.COLOR_BGR2GRAY)
                self.timestamp_update = time.time_ns()
                # print(f'   #UPDATE() timestamp for frame capture.read() {self.timestamp_update}')
            time.sleep(.1)

    def show_frame(self, win_name='frame_1', display_text=None, stream_vals=None):
        '''
        Display frames in the main window
        :param win_name: Window name title
        :param display_text: Display text
        :return: self.histogram
        '''
        self.display_text = display_text
        self.font_face = cv2.FONT_HERSHEY_SIMPLEX  # cv2.FONT_HERSHEY_TRIPLEX # FONT_HERSHEY_SCRIPT_SIMPLEX #FONT_HERSHEY_COMPLEX
        self.text_origin = (50, 450)  # (X,Y)
        self.text_fontScale = 1.0
        self.text_colour = (255, 0, 0)
        self.text_thickness = 1
        self.line_thickness = 5
        self.stream_vals = stream_vals

        self.numPixels = np.prod(self.frame.shape[:2])  # Normalize histogram based on number of pixels per frame.
        self.histogram = cv2.calcHist([self.frame], [0], None, [16], [0, 255]) / self.numPixels

        # display display_text
        cv2.putText(img=self.frame, text=self.display_text, org=self.text_origin,
                    fontFace=self.font_face, fontScale=self.text_fontScale,
                    color=self.text_colour, thickness=self.text_thickness)

        # Display histogram values
        # cv2.putText(img=self.frame, text=str(self.histogram), org=(50, 400),
        #             fontFace=self.font_face, fontScale=0.25,
        #             color=self.text_colour, thickness=self.text_thickness)

        # Adding bin0 as line
        # b0 = 300+int(100*self.histogram[0])
        # cv2.line(self.frame, (50, 300), (50, b0), (255, 0, 0), self.line_thickness)

        cv2.imshow(win_name, self.frame)
        self.videowriter.write(self.frame)
        # print(self.stream_vals[0], self.stream_vals[1], self.stream_vals[2])
        streamed_values.append(self.stream_vals)
        streamed_values_video_timestamp.append(self.timestamp_update)
        # print(f'   ##SHOW_FRAME() timestamp for frame capture.read() {self.timestamp_update}')

        key = cv2.waitKey(1)
        if key == ord('q'):
            print('[Q]uit capture data')
            self.capture.release()
            cv2.destroyAllWindows()
            self.videowriter.release()

            # Recording streamed sensor values in *.csv file
            with open(self.video_filename + '.csv', 'w', newline='', encoding='utf-8') as csvfile:
                fieldnames = ['Sample number',
                              'epoch machine time (ns)',
                              'Timestamp for frame capture.read  (ns)',
                              'Timestamp LPMSB2 (s)',
                              'Quaternions [q0, q1, q2, q3] LPMS-B2',
                              'Euler [Roll, Pitch, Yaw] LPMSB2'
                              ]
                writer = csv.writer(csvfile)
                writer.writerow(fieldnames)
                for idx in range(len(streamed_values)):
                    # print(i, streamed_values[i][0])
                    writer.writerow([
                        idx,  # 'Sample number',
                        streamed_values[idx][0],  # time.time_ns(), 'epoch machine time (ns)',
                        streamed_values_video_timestamp[idx],  # 'Timestamp for frame capture.read  (ns)',
                        streamed_values[idx][1],  # str(imu_data.timestamp), 'Timestamp LPMSB2 (s)',
                        streamed_values[idx][2],  # str(imu_data.q), 'Quaternions [q0, q1, q2, q3] LPMS-B2',
                        streamed_values[idx][3]  # str(imu_data.r), 'Euler [Roll, Pitch, Yaw] LPMSB2'
                    ])
                    # TODO tests: time.sleep(5)

            print("Streaming of sensor data complete")
            sensor.release()
            client.close()
            print("OpenZen library was closed")
            exit(1)

        return self.histogram

    def get_settings(self):
        """
        Listing available frame formats for device video0: uvcdynctrl -f
        :return:
        """
        self.fps = self.capture.get(cv2.CAP_PROP_FPS)
        self.w = self.capture.get(cv2.CAP_PROP_FRAME_WIDTH)
        self.h = self.capture.get(cv2.CAP_PROP_FRAME_HEIGHT)
        self.fcc = int(self.capture.get(cv2.CAP_PROP_FOURCC))
        self.bs = self.capture.get(cv2.CAP_PROP_BUFFERSIZE)
        print(f'-------------------------------------------')
        print(f'------  Getting capture parameters  -------')
        print(f'fps: {self.fps}')
        print(f'resolution: WIDTH:{self.w} HEIGHT{self.h}')
        print(f'codec: {[chr((int(self.fcc) >> 8 * i) & 0xFF) for i in range(4)]}')
        print(f'Buffer size: {self.bs}')


def plot_histogram_initialisation():
    # Initialize plot line object(s). Turn on interactive plotting and show plot.
    color = 'gray'
    bins = 16
    # Initialize plot.
    fig, ax = plt.subplots()
    ax.set_title('Histogram (grayscale)')
    ax.set_xlabel('Bin')
    ax.set_ylabel('Frequency')

    lw = 3
    alpha = 0.5
    lineGray, = ax.plot(np.arange(bins), np.zeros((bins, 1)), c='k', lw=lw, label='intensity')
    ax.set_xlim(0, bins - 1)
    ax.set_ylim(0, 1)
    ax.legend()
    plt.ion()
    plt.show()


#####################################################
### START OF Setting up LPMS-B2
start = timer()

openzen.set_log_level(openzen.ZenLogLevel.Warning)

error, client = openzen.make_client()
if not error == openzen.ZenError.NoError:
    print("Error while initializinng OpenZen library")
    sys.exit(1)

error = client.list_sensors_async()

# check for events
sensor_desc_connect = None
while True:
    zenEvent = client.wait_for_next_event()

    if zenEvent.event_type == openzen.ZenEventType.SensorFound:
        print("Found sensor {} on IoType {}".format(zenEvent.data.sensor_found.name,
                                                    zenEvent.data.sensor_found.io_type))
        if sensor_desc_connect is None:
            sensor_desc_connect = zenEvent.data.sensor_found

    if zenEvent.event_type == openzen.ZenEventType.SensorListingProgress:
        lst_data = zenEvent.data.sensor_listing_progress
        print("Sensor listing progress: {} %".format(lst_data.progress * 100))
        if lst_data.complete > 0:
            break
print("Sensor Listing complete")

if sensor_desc_connect is None:
    print("No sensors found")
    sys.exit(1)

# connect to the first sensor found
# error, sensor = client.obtain_sensor(sensor_desc_connect)

# or connect to a sensor by name
# error, sensor = client.obtain_sensor_by_name("LinuxDevice", "LPMSCU2000003")
error, sensor = client.obtain_sensor_by_name("Bluetooth", "00:04:3E:53:ED:5B", 921600)

if not error == openzen.ZenSensorInitError.NoError:
    print("Error connecting to sensor")
    sys.exit(1)

print("Connected to sensor !")

imu = sensor.get_any_component_of_type(openzen.component_type_imu)
if imu is None:
    print("No IMU found")
    sys.exit(1)

## read bool property
error, is_streaming = imu.get_bool_property(openzen.ZenImuProperty.StreamData)
if not error == openzen.ZenError.NoError:
    print("Can't load streaming settings")
    sys.exit(1)

print("Sensor is streaming data: {}".format(is_streaming))
print("\n>> Set and get IMU settings")

imu = sensor.get_any_component_of_type(openzen.component_type_imu)
if imu is None:
    print("No IMU found")
    sys.exit(1)

## read bool property
error, is_streaming = imu.get_bool_property(openzen.ZenImuProperty.StreamData)
if not error == openzen.ZenError.NoError:
    print("Can't load streaming settings")
    sys.exit(1)

# set and read SamplingRate (Available sample rate of the IMU: 5, 10, 25, 50, 100, 200, 400)
error = imu.set_int32_property(openzen.ZenImuProperty.SamplingRate, 200)
error, freq = imu.get_int32_property(openzen.ZenImuProperty.SamplingRate)
print(f'Sampling rate: {freq}')

end = timer()
print("\nSetting Sensor Time Interval: {} s".format(end - start))

### END OF Setting up LPMS-B2
#####################################################


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--idFG', required=True, help='Specify ID for framegrabber', type=int)
    parser.add_argument('--fps', required=True, help='Specify frame per second', type=int)
    parser.add_argument('--fW', required=True, help='Specify width of the image', type=int)
    parser.add_argument('--fH', required=True, help='Specify high of the image', type=int)
    parser.add_argument('--vfn', required=True, help='Specify video file name', type=str)
    args = parser.parse_args()

    video_stream_widget = VideoStreamWidget(id_framegrabber=args.idFG,
                                            frames_per_second=args.fps,
                                            frame_width=args.fW,
                                            frame_height=args.fH,
                                            video_filename=args.vfn
                                            )
    video_stream_widget.get_settings()
    WINDOW_TITLE = 'Video of USB endoscope and pose'

    sample_number = 0
    streamed_values = []
    streamed_values_video_timestamp = []
    # machine_timestamp_0 = []

    print(f'--------------------------')
    print(f'  Start of the main loop  ')

    while True:
        t1 = time.time_ns()
        zenEvent = client.wait_for_next_event()
        # check if its an IMU sample event and if it
        # comes from our IMU and sensor component
        if zenEvent.event_type == openzen.ZenEventType.ImuData and \
                zenEvent.sensor == imu.sensor and \
                zenEvent.component.handle == imu.component.handle:
            imu_data = zenEvent.data.imu_data
            # machine_timestamp_0 = timer()
            machine_timestamp_after_imu_data = time.time_ns()
            # time.time() Return the time in seconds since the epoch as a floating point number.
            # Similar to time() but returns time as an integer number of nanoseconds since the epoch.

            # print(f'     Timestamp for zenEvent.data.imu_data {machine_timestamp_after_imu_data}')

            # Printing IMU values will adds delays
            # print(f'    --------------------------------------------')
            # print(f' LPMS-B2 timestamp [s]: {imu_data.timestamp}')
            # print(f'            A [m/s^2]: {imu_data.a}')
            # print(f'            G [deg/s]: {imu_data.g1} ')
            # print(f' Quaternion [no unit]: {imu_data.q}')
            # print(f'        Euler [deg/s]: {imu_data.r}')
            # See more data descriptions
            # https://lpresearch.bitbucket.io/openzen/latest/data.html

            # q2e.append(imu_data.q)
            # timestamps.append(imu_data.timestamp)
            # n += 1

            DISPLAY_TEXT = 'Euler A:' + str(round(imu_data.r[0], 2)) + \
                           '  B:' + str(round(imu_data.r[1], 2)) + \
                           '  G:' + str(round(imu_data.r[2], 2))

            print(f'.')
            # print(f'{sample_number}')
            # print(f'IMU sample_number: {sample_number}')
            sample_number += 1  # sample_number + 1

            try:
                histogram_frame = video_stream_widget.show_frame(
                    win_name=WINDOW_TITLE, \
                    display_text=None, \
                    stream_vals=[
                        machine_timestamp_after_imu_data,
                        str(imu_data.timestamp),
                        str(imu_data.q),
                        str(imu_data.r)
                    ]
                )
                # TODO: Debug the following 4 lines
                # video_stream_widget.record_data(timestamps)
                # video_stream_widget.record_data(imu_data.timestamp)
                # lineGray.set_ydata(histogram_frame)
                # fig.canvas.draw()
            except AttributeError:
                pass

    # # TODO: ERRORS
    # #     except KeyboardInterrupt: #AttributeError: 'VideoStreamWidget' object has no attribute 'frame'
    # #         pass
    #
    # # print("\nRun Time: {} s".format(end - start))
    # # print(start)
    # # print(end)
