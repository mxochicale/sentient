"""
tested on Jun 23 2023
python: 3.10.12 | packaged by conda-forge | (main, Jun 23 2023, 22:40:32) [GCC 12.3.0]
opencv: 4.8.0
torch: 2.0.0.post200
torch cuda_is_available: True
torch cuda version: 11.2
torch cuda.device_count  1
PIL version: 10.0.0
Tensorflow version: 2.12.1
"""

import sys

import cv2
import PIL

# import torch
# import tensorflow as tf
# import h5py
# import albumentations
# import sklearn
# import plotly
# import pandas


def test_packages_versions():
    """
    test packages versions
    """
    print("\n")
    print("Package versions")
    print(f"python: {sys.version}")
    print(f"opencv: {cv2.__version__}")
    print(f"PIL version: {PIL.__version__}")
    # print(f"torch: {torch.__version__}")
    # print(f"torch cuda_is_available: {torch.cuda.is_available()}")
    # print(f"torch cuda version: {torch.version.cuda}")
    # print(f"torch cuda.device_count  {torch.cuda.device_count()}")
    # print(f'h5py: {h5py.__version__}')
    # print(f'albumentations: {albumentations.__version__}')
    # print(f'sklearn version: {sklearn.__version__}')
    # print(f'plotly version: {plotly.__version__}')
    # print(f'pandas version: {pandas.__version__}')
    # print(f"Tensorflow version: {tf.__version__}")
