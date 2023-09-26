# Slides

## Abstract 
"Towards lightweight transformer-based models with multimodal data for low-latency surgical applications" has been accepted for 15 minutes presentation at the Fast Machine Learning for Science!
Abstract: Surgical data technologies have not only been successfully integrated inputs from various data sources (e.g., medical devices, trackers, robots and cameras) but have also applied a range of machine learning and deep learning methods (e.g., classification, segmentation or synthesis) to data-driven interventional healthcare. However, the diversity of data, acquisitions and pre-processing methods, data types, as well as training and inference methods has presented a challenging scenario for implementing low-latency applications in surgery. Recently, transformers-based models have emerged as dominant neural networks, owing to their attention mechanisms and parallel capabilities when using multimodal medical data. Despite this progress, state-of-the-art transformers-based models remain heavyweight and challenging to optimise (with 100MB of parameters) for real-time applications. Hence, in this work, we concentrate on a lightweight transformer-based model and employ pruning techniques to achieve a balance in data size for both training and testing workflows, aiming at enhancing real-time performance. We present preliminary results from a machine learning workflow designed for real-time classification of surgical skills assessment. We similarly present a reproducible workflow for data collection using multimodal sensors, including USB video image and Bluetooth-based inertial sensors. This highlights the potential of applying models with small memory and parameter size, enhancing inference speed for surgical applications. Code, data and other resources to reproduce this work are available at https://github.com/mxochicale/rtt4ssa

Further details of the talk: https://indico.cern.ch/event/1283970/contributions/5550640/
Full program: https://indico.cern.ch/event/1283970/timetable/#20230926.detailed

## Timing
You have been allocated either 15 minutes (standard) or 5 minutes (lightning) for your talk.

## Questions from the audience: 
1. Do you know what's the inference seed of your model?
2. What latency values are expecting in clinical settings?

Some related work Exploring medical applications of fast ML with a novel FPGA firmware framework"" >  https://indico.cern.ch/event/1283970/contributions/5550639/ 

## Building tex abstract with:
Commit changes
```
git add -A
git commit -m 'genesis of slides'
git push origin generated-pdfs
```

## Local build
### Requirements 
* Install latest version of (i.e., Tex Live 2020 [:link:](https://github.com/mxochicale/latex/tree/master/installation)).
* sudo apt-get install python-pygments #https://tex.stackexchange.com/questions/40083/how-to-install-minted-in-ubuntu

## local build
make clean && make && evince main.pdf

