# conda activate *VE
# python package_versions.py
import sys
import cv2
import torch
#import h5py
#import albumentations
#import sklearn
#import plotly
import PIL
#import pandas
import tensorflow as tf

print(f'python: {sys.version}')
print(f'opencv: {cv2.__version__}')
print(f'torch: {torch.__version__}')
print(f'torch cuda_is_available: {torch.cuda.is_available()}')
print(f'torch cuda version: {torch.version.cuda}')
print(f'torch cuda.device_count  {torch.cuda.device_count()}')
#print(f'h5py: {h5py.__version__}')
#print(f'albumentations: {albumentations.__version__}')
#print(f'sklearn version: {sklearn.__version__}')
#print(f'plotly version: {plotly.__version__}')
print(f'PIL version: {PIL.__version__}')
#print(f'pandas version: {pandas.__version__}')
print(f'Tensorflow version: {tf.__version__}')
