from sentient.utils.utils import video_to_tensor, compute_texture_array_and_plot
from sentient.utils.utils import data_frame_of_texture_analysis
from sentient.utils.utils import get_and_plot_imu_data_analysis

import os
import time
import pandas as pd
import numpy as np
import torch
import matplotlib.pyplot as plt
import cv2
import skimage
from typing import Tuple, List

HOME_PATH = os.path.expanduser(f'~')
USERNAME = os.path.split(HOME_PATH)[1]
REPOSITORY_PATH='repositories/sentient'

###########################
###SETTING DATA_PATHS
#DATA_PATH='repositories/datasets/in2research2023/Thu-27-Jul-2023' 

RAW_DATA_PATH='repositories/datasets/in2research2023/Thu-24-Aug-2023'
PREPROCESSED_DATA_PATH='repositories/datasets/in2research2023/Thu-24-Aug-2023-preprocessed'

FULL_REPO_DATA_PATH = HOME_PATH +'/' + RAW_DATA_PATH
FULL_REPO_PREPROCESSED_DATA_PATH = HOME_PATH +'/' + PREPROCESSED_DATA_PATH +'/'
os.makedirs(FULL_REPO_PREPROCESSED_DATA_PATH, exist_ok=True) 

## Printing Versions and paths
#print(FULL_REPO_DATA_PATH)
#print(f'PyTorch Version: {torch.__version__}')
#print(f'pandas Version: {pd.__version__}')
#print(f'numpy Version: {np.__version__}')
#print(f'cv2 Version: {cv2.__version__}')
#print(f'skimage Version: {skimage.__version__}')


#PARTICIPANTNN = 'participant01'
#PARTICIPANTNN_TESTNN = 'participant01-test01-rep01-1g-5mins' #51,328
#PARTICIPANTNN_TESTNN = 'participant01-test01-rep02-1g-5mins' #51,178
#PARTICIPANTNN_TESTNN = 'participant01-test02-rep01-1g-5mins' #49,183
#PARTICIPANTNN_TESTNN = 'participant01-test02-rep02-1g-5mins' #47,577
#PARTICIPANTNN_TESTNN = 'participant01-test03-rep01-1g-5mins' #48,688
#PARTICIPANTNN_TESTNN = 'participant01-test03-rep02-1g-5mins'#48,789

PARTICIPANTNN = 'participant02'
#PARTICIPANTNN_TESTNN = 'participant02-test01-rep01-1g-5mins'#49,490
#PARTICIPANTNN_TESTNN = 'participant02-test01-rep02-1g-5mins'#49,219
#PARTICIPANTNN_TESTNN = 'participant02-test02-rep01-1g-5mins'#48,043
#PARTICIPANTNN_TESTNN = 'participant02-test02-rep02-1g-5mins'#49,606
#PARTICIPANTNN_TESTNN = 'participant02-test03-rep01-1g-5mins'#48,875
PARTICIPANTNN_TESTNN = 'participant02-test03-rep02-1g-5mins'#48,050

start_frame_number = 0
end_frame_number = 40000 #(resulted samples are end_frame_number-2)
display_factor_for_texture_analysis_array = 100000


CSV_FILENAME_FOR_TEXTURE_ANALYSIS=PARTICIPANTNN_TESTNN+'.csv'
FULL_PATH_AND_AVI_FILE = os.path.join(FULL_REPO_DATA_PATH, PARTICIPANTNN, PARTICIPANTNN_TESTNN+'.avi')
FULL_PATH_AND_CSV_FILE = os.path.join(FULL_REPO_DATA_PATH, PARTICIPANTNN, PARTICIPANTNN_TESTNN+'.avi.csv')

total_number_of_frames = end_frame_number - start_frame_number


start_time = time.time()



video, frames_timestam = video_to_tensor(FULL_PATH_AND_AVI_FILE, start_frame_number, end_frame_number)

num_frames, height, width = video.shape
#print(f'num_frames: {num_frames}')
#print(f'height: {height}')
#print(f'width: {width}')

display_figures=False
#display_figures=True

texture_analysis_array = compute_texture_array_and_plot(video, frames_timestam, display_figures, display_factor_for_texture_analysis_array)

df_texture_analysis = data_frame_of_texture_analysis(texture_analysis_array, start_frame_number, end_frame_number)
df, ndf, nqdf = get_and_plot_imu_data_analysis(FULL_PATH_AND_CSV_FILE, start_frame_number, end_frame_number, display_figures)

#df_a = df_texture_analysis[['frame_i', 'Contrast_normalised', 'Correlation_normalised', 'Dissimilarity_normalised', 'Energy_normalised', 'Homogeneity_normalised', 'ASM_normalised']]
df_a = df_texture_analysis[['frame_i', 'Contrast_normalised', 'Correlation_normalised', 'Dissimilarity_normalised', 'Homogeneity_normalised']]


df_b = df[['q0', 'q1', 'q2', 'q3']]
dff = pd.concat([df_a, df_b], axis=1)

dff.to_csv(FULL_REPO_PREPROCESSED_DATA_PATH+PARTICIPANTNN_TESTNN+'_normalised_quaternions'+'.csv', index=False) 


end_time = time.time()
execution_time = end_time - start_time
print(f'Execution time: {execution_time/60} minutes')

