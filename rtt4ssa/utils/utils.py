import cv2
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from skimage.feature import graycomatrix, graycoprops
from typing import Tuple


def msec_to_timestamp(current_timestamp: float) -> Tuple[float]:
    """
    Convert millisecond variable to a timestamp variable with the format minutes, seconds and milliseconds
    """
    minutes = int(current_timestamp / 1000 / 60)
    seconds = int(np.floor(current_timestamp / 1000) % 60)
    ms = current_timestamp - np.floor(current_timestamp / 1000) * 1000

    return minutes, seconds, '{:.3f}'.format(ms), '{:02d}:{:02d}:{:.3f}'.format(minutes, seconds, ms)


def masks_us_image(image_frame_array_1ch: np.ndarray) -> np.ndarray:
    """
    Hard mask pixels outside of scanning sector
    """
    mask = np.zeros_like(image_frame_array_1ch)

    #                 top-left,
    #                     top-right,
    #                           bottom-right
    #                                             arc
    #                                                                         bottom-left

    # x_data = np.array([250, 380, 572,           454,321,165                   ,60])
    # y_data = np.array([30, 30, 320,             389,421,382                   ,320 ])

    x_data = np.array([250, 380, 572,
                       597, 580, 561, 537, 515, 491, 474, 436, 395, 366, 333, 293, 249, 207, 166, 97, 77, 61, 42  # arc
                          , 60])  # bottom-left
    y_data = np.array([30, 30, 320,
                       368, 380, 392, 404, 416, 424, 430, 440, 448, 451, 452, 453, 446, 439, 426, 398, 388, 380, 366
                       # arc
                          , 320])  # bottom-left
    scan_arc_mask_v01 = np.vstack((x_data, y_data)).astype(np.int32).T

    # caliper_scale_mask = np.array([(1770, 120), (1810, 120), (1810, 930), (1770, 930)])
    cv2.fillPoly(mask, [scan_arc_mask_v01],
                 (255, 255, 0))
    maskedImage = cv2.bitwise_and(image_frame_array_1ch, image_frame_array_1ch, mask=mask)

    return maskedImage


def GLCMs(img):
    """
    Calculating gray level co-occurrence matrices (GLCMs)
    https://scikit-image.org/docs/dev/auto_examples/features_detection/
           plot_glcm.html#sphx-glr-auto-examples-features-detection-plot-glcm-py

    Haralick, RM.; Shanmugam, K., “Textural features for image classification”
        IEEE Transactions on systems, man, and cybernetics 6 (1973): 610-621.
        [DOI:10.1109/TSMC.1973.4309314](https://doi.org/10.1109/TSMC.1973.4309314)
    PDF: http://haralick-org.torahcode.us/journals/TexturalFeaturesHaralickShanmugamDinstein.pdf
    google-citations: https://scholar.google.com/scholar?cites=
                        13863271628667072083&as_sdt=2005&sciodt=0,5&hl=en

    """
    return graycomatrix(img, [1], [0], levels=256)


def Contrast(R):
    ct = graycoprops(R, prop='contrast')
    return ct


def Correlation(R):
    cn = graycoprops(R, prop='correlation')
    return cn


def Dissimilarity(R):
    d = graycoprops(R, prop='dissimilarity')
    return d


def Energy(R):
    e = graycoprops(R, prop='energy')
    return e


def Homogeneity(R):
    h = graycoprops(R, prop='homogeneity')
    return h


def ASM(R):
    a = graycoprops(R, prop='ASM')
    return a


def video_to_tensor(FULL_PATH_AND_AVI_FILE, start_frame_number, end_frame_number):
    cap = cv2.VideoCapture(FULL_PATH_AND_AVI_FILE)
    if cap.isOpened() == False:
        print('[ERROR] [ViewVideoDataset.__getitem__()] Unable to open video ' + FULL_PATH_AND_AVI_FILE)
        exit(-1)

    # Get parameters of input video
    frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = int(np.ceil(cap.get(cv2.CAP_PROP_FPS)))
    frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    # Print video features
    print(f'  ')
    print(f'  ')
    print(f'  VIDEO_FEATURES')
    print(f'    video_name={FULL_PATH_AND_AVI_FILE}')
    print(f'    Frame_height={frame_height}, frame_width={frame_width} fps={fps} nframes={frame_count} ')
    print(f'  ')
    print(f'  ')
    # # #         if start_frame_number >= end_frame_number:
    # # #             raise Exception("start frame number must be less than end frame number")

    cap.set(cv2.CAP_PROP_POS_FRAMES, start_frame_number)
    frames_numpy_ndarray = []
    frames_torch = []
    frames_timestamp = []

    # pbar = tqdm(total=total_number_of_frames - 1)
    while cap.isOpened():
        success, image_frame_3ch_i = cap.read()

        if not success and len(frames_torch) < 1:
            print(f'[ERROR] {FULL_PATH_AND_AVI_FILE}')
            exit(-1)
            break

        if cap.get(cv2.CAP_PROP_POS_FRAMES) >= end_frame_number:
            break

        frame_msec = cap.get(cv2.CAP_PROP_POS_MSEC)
        current_frame_timestamp = msec_to_timestamp(frame_msec)
        # print(current_frame_timestamp)#(0, 33, '333.333', '00:33:333.333')
        # print(current_frame_timestamp[2])#'00:33:333.333'
        # print(current_frame_timestamp[3])#00:33:333.333

        # (H x W x C) to (C x H x W)
        # print(type(image_frame_3ch_i))#    <class 'numpy.ndarray'>
        # print(image_frame_3ch_i.shape)#(480, 640, 3)
        image_frame_1ch_i = cv2.cvtColor(image_frame_3ch_i, cv2.COLOR_BGR2GRAY)

        frames_numpy_ndarray.append(image_frame_1ch_i)
        # frames_numpy_ndarray.append(masks_us_image(image_frame_1ch_i))

        frames_timestamp.append(current_frame_timestamp[3])

    #     frame_torch = torch.from_numpy(image_frame_3ch_i).float()
    #     frame_torch = frame_torch.squeeze()  # Fake batch dimension to be "H,W,C"
    #     print(type(frame_torch))#<class 'torch.Tensor'>
    #     print(frame_torch.shape)#torch.Size([480, 640, 3])

    # # #             cropped_image_frame_ = cropped_frame(masked_frame, self.crop_bounds)
    # # #             frame_torch = ToImageTensor(cropped_image_frame_)

    video = np.stack(frames_numpy_ndarray, axis=0)  # dimensions (Fi, H, W, C)
    cap.release()

    return video, frames_timestamp


def display_videoframe_and_hist(im):
    # im = video[frame_i]
    fig = plt.figure(dpi=50, figsize=(15, 5))

    ax0 = fig.add_subplot(1, 2, 1)
    ax0.imshow(im, cmap='gray')

    ax1 = fig.add_subplot(1, 2, 2)

    # im = np.ravel(im)
    # im = im[np.nonzero(im)]  # Ignore the background
    # im = im / (2**16 - 1)  # Normalize
    # ax1.hist(im, bins=10)
    # https://matplotlib.org/stable/gallery/specialty_plots/
    # mri_with_eeg.html#sphx-glr-gallery-specialty-plots-mri-with-eeg-py

    im = np.ravel(im) / 256  # Normalize
    im = im[np.nonzero(im)]  # Ignore the background
    ax1.hist(im, bins=256, range=(0.0, 1.0))
    # https://matplotlib.org/stable/tutorials/introductory/images.html#sphx-glr-tutorials-introductory-images-py

    ax1.minorticks_on()
    ax1.set_xlabel('Normalised Intensity Values')
    ax1.set_ylabel('Normalised Density')

    plt.tight_layout()
    plt.show()


def compute_texture_array_and_plot(video, frames_timestam, display_interval):
    """
    O'Byrne, Michael, Bidisha Ghosh, Vikram Pakrashi, and Franck Schoefs.
    "Texture analysis based detection and classification of surface features on ageing infrastructure elements."
    In BCRI2012 Bridge & Concrete Research in Ireland. 2012.
    https://hal.science/hal-01009012/document
    """
    texture_analysis_array = []
    for frame_i in range(len(video)):
        image_frame = video[frame_i]

        # Compute Grey-Level-Co-occurrence Matrix
        R = GLCMs(image_frame)

        # Calculating texture property Contrast
        con = Contrast(R)

        # Calculating texture property Correlation
        cor = Correlation(R)

        # Calculating texture property Dissimilarity
        dis = Dissimilarity(R)

        # Calculating texture property Energy
        en = Energy(R)

        # Calculating texture property Homogeneity
        homo = Homogeneity(R)

        # Calculating texture property Angular Second Moment (ASM)
        asm = ASM(R)

        texture_analysis_array.append([con, cor, dis, en, homo, asm])
        # print(con,cor,dis,en,homo, asm)

        if frame_i % display_interval == 0:
            print(f'frame_i: {frame_i}, timestamp {frames_timestam[frame_i]}')
            display_videoframe_and_hist(image_frame)

        # plt.savefig('filename'+str(frame_i)+'.png', dpi=300)

    return texture_analysis_array


def data_frame_of_texture_analysis(texture_analysis_array, start_frame_number, end_frame_number):
    texture_analysis_np_array = np.stack(texture_analysis_array, axis=0)
    texture_analysis_np_array = texture_analysis_np_array.transpose()
    texture_analysis_np_array = texture_analysis_np_array.squeeze()

    df_texture_analysis = pd.DataFrame(
        {
	    'frame_i': np.arange(start_frame_number, end_frame_number - 1),
            'Contrast': texture_analysis_np_array[0],
            'Correlation': texture_analysis_np_array[1],
            'Dissimilarity': texture_analysis_np_array[2],
            'Energy': texture_analysis_np_array[3],
            'Homogeneity': texture_analysis_np_array[4],
            'ASM': texture_analysis_np_array[5]
        }
    )

    # Mean normalisation
    df_texture_analysis["Contrast_normalised"] = \
        (df_texture_analysis["Contrast"] - df_texture_analysis["Contrast"].mean()) / df_texture_analysis["Contrast"].std()
    df_texture_analysis["Correlation_normalised"] = \
        (df_texture_analysis["Correlation"] - df_texture_analysis["Correlation"].mean()) / df_texture_analysis["Correlation"].std()
    df_texture_analysis["Dissimilarity_normalised"] = \
        (df_texture_analysis["Dissimilarity"] - df_texture_analysis["Dissimilarity"].mean()) / df_texture_analysis["Dissimilarity"].std()
    df_texture_analysis["Energy_normalised"] = \
        (df_texture_analysis["Energy"] - df_texture_analysis["Energy"].mean()) / df_texture_analysis["Energy"].std()
    df_texture_analysis["Homogeneity_normalised"] = \
        (df_texture_analysis["Homogeneity"] - df_texture_analysis["Homogeneity"].mean()) / df_texture_analysis["Homogeneity"].std()
    df_texture_analysis["ASM_normalised"] = \
        (df_texture_analysis["ASM"] - df_texture_analysis["ASM"].mean()) / df_texture_analysis["ASM"].std()

    return df_texture_analysis


def get_and_plot_imu_data_analysis(FULL_PATH_AND_CSV_FILE):
    df = pd.read_csv(FULL_PATH_AND_CSV_FILE)
    # print(df)

    # # df = df.rename(columns={'Euler_computed [Roll, Pitch, Yaw]': 'Euler_computed'})
    df = df.rename(columns={'Sample number': 'Sample_number'})
    df = df.rename(columns={'Euler [Roll, Pitch, Yaw] LPMSB2': 'Euler_LPMSB2'})
    df = df.rename(columns={'Quaternions [q0, q1, q2, q3] LPMS-B2': 'Quaternions_LPMSB2'})

    df[['A', 'B', 'C']] = df.Euler_LPMSB2.str.split(',', expand=True)
    df['A'] = df['A'].apply(lambda x: x.replace('[', ''))
    df['C'] = df['C'].apply(lambda x: x.replace(']', ''))
    df['A'] = pd.to_numeric(df['A'], errors='coerce')
    df['B'] = pd.to_numeric(df['B'], errors='coerce')
    df['C'] = pd.to_numeric(df['C'], errors='coerce')

    df[['q0', 'q1', 'q2', 'q3']] = df.Quaternions_LPMSB2.str.split(',', expand=True)
    df['q0'] = df['q0'].apply(lambda x: x.replace('[', ''))
    df['q3'] = df['q3'].apply(lambda x: x.replace(']', ''))
    df['q0'] = pd.to_numeric(df['q0'], errors='coerce')
    df['q1'] = pd.to_numeric(df['q1'], errors='coerce')
    df['q2'] = pd.to_numeric(df['q2'], errors='coerce')
    df['q3'] = pd.to_numeric(df['q3'], errors='coerce')

    # print(df.head())# #Print head of csv

    ## EULER ANGLES
    ndf_a = pd.DataFrame(data=df['Sample_number'])
    ndf_a.insert(1, 'Euler_angle', str('alpha'), True)
    ndf_a.insert(2, 'Euler_val', df['A'], True)

    ndf_b = pd.DataFrame(data=df['Sample_number'])
    ndf_b.insert(1, 'Euler_angle', str('beta'), True)
    ndf_b.insert(2, 'Euler_val', df['B'], True)

    ndf_c = pd.DataFrame(data=df['Sample_number'])
    ndf_c.insert(1, 'Euler_angle', str('gamma'), True)
    ndf_c.insert(2, 'Euler_val', df['C'], True)

    ndf = pd.concat([ndf_a, ndf_b, ndf_c], ignore_index=True)
    # print(ndf)

    sns.lineplot(data=ndf, x='Sample_number', y='Euler_val', hue='Euler_angle', lw=2)
    plt.show()

    ## QUATERNIONS
    nqdf_q0 = pd.DataFrame(data=df['Sample_number'])
    nqdf_q0.insert(1, 'Quaternion', str('q0'), True)
    nqdf_q0.insert(2, 'Q_val', df['q0'], True)

    nqdf_q1 = pd.DataFrame(data=df['Sample_number'])
    nqdf_q1.insert(1, 'Quaternion', str('q1'), True)
    nqdf_q1.insert(2, 'Q_val', df['q1'], True)

    nqdf_q2 = pd.DataFrame(data=df['Sample_number'])
    nqdf_q2.insert(1, 'Quaternion', str('q2'), True)
    nqdf_q2.insert(2, 'Q_val', df['q2'], True)

    nqdf_q3 = pd.DataFrame(data=df['Sample_number'])
    nqdf_q3.insert(1, 'Quaternion', str('q3'), True)
    nqdf_q3.insert(2, 'Q_val', df['q3'], True)

    nqdf = pd.concat([nqdf_q0, nqdf_q1, nqdf_q2, nqdf_q3], ignore_index=True)
    # print(nqdf)

    sns.lineplot(data=nqdf, x='Sample_number', y='Q_val', hue='Quaternion', lw=2)
    plt.show()

    return df, ndf, nqdf
