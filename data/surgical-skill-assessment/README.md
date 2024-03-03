# Demo data for surgical-skill-assessment 

Demo datasets (1.3 GB) can be downloaded from zenodo: https://doi.org/10.5281/zenodo.10775099.
See further details [here](../docs/protocols/experiment-24-aug-2023/README.md) on its collection.
Notes. you can host up to 50GB of data in zenodo. 

## Demo data Thu-24-Aug-2023
Tree of demo dataset. Each pair of video and time-series were recorded for approximately 5 minutes. 
```
$ cd $HOME/repositories/datasets/in2research2023/Thu-24-Aug-2023
$ tree -s
[       4096]  .
├── [       4096]  participant01
│   ├── [ 1126670892]  participant01-test01-rep01-1g-5mins.avi
│   ├── [   10503488]  participant01-test01-rep01-1g-5mins.avi.csv
│   ├── [ 1093617412]  participant01-test01-rep02-1g-5mins.avi
│   ├── [   10490777]  participant01-test01-rep02-1g-5mins.avi.csv
│   ├── [ 1540763444]  participant01-test02-rep01-1g-5mins.avi
│   ├── [   10083571]  participant01-test02-rep01-1g-5mins.avi.csv
│   ├── [ 1503624576]  participant01-test02-rep02-1g-5mins.avi
│   ├── [    9792205]  participant01-test02-rep02-1g-5mins.avi.csv
│   ├── [ 1289063688]  participant01-test03-rep01-1g-5mins.avi
│   ├── [    9975193]  participant01-test03-rep01-1g-5mins.avi.csv
│   ├── [ 1260531560]  participant01-test03-rep02-1g-5mins.avi
│   └── [   10033743]  participant01-test03-rep02-1g-5mins.avi.csv
└── [       4096]  participant02
    ├── [ 1251925628]  participant02-test01-rep01-1g-5mins.avi
    ├── [   10188391]  participant02-test01-rep01-1g-5mins.avi.csv
    ├── [ 1241199998]  participant02-test01-rep02-1g-5mins.avi
    ├── [   10043427]  participant02-test01-rep02-1g-5mins.avi.csv
    ├── [ 1423517518]  participant02-test02-rep01-1g-5mins.avi
    ├── [    9913693]  participant02-test02-rep01-1g-5mins.avi.csv
    ├── [ 1283264068]  participant02-test02-rep02-1g-5mins.avi
    ├── [   10211794]  participant02-test02-rep02-1g-5mins.avi.csv
    ├── [ 1315188186]  participant02-test03-rep01-1g-5mins.avi
    ├── [   10055324]  participant02-test03-rep01-1g-5mins.avi.csv
    ├── [ 1437222374]  participant02-test03-rep02-1g-5mins.avi
    └── [    9870705]  participant02-test03-rep02-1g-5mins.avi.csv

2 directories, 24 files
```

### Pre-processed data
The following files were created by running `python A_preprocessing_data_from_multiple-files.py` each file contains 40K samples and took around 4 minutes for execution time. 
```
$ cd $HOME/repositories/datasets/in2research2023/Thu-24-Aug-2023-preprocessed
$ tree -s
[       4096]  .
├── [    6478131]  participant01-test01-rep01-1g-5mins_normalised_quaternions.csv
├── [    6474319]  participant01-test01-rep02-1g-5mins_normalised_quaternions.csv
├── [    6467386]  participant01-test02-rep01-1g-5mins_normalised_quaternions.csv
├── [    6484423]  participant01-test02-rep02-1g-5mins_normalised_quaternions.csv
├── [    6460637]  participant01-test03-rep01-1g-5mins_normalised_quaternions.csv
├── [    6494165]  participant01-test03-rep02-1g-5mins_normalised_quaternions.csv
├── [    6453894]  participant02-test01-rep01-1g-5mins_normalised_quaternions.csv
├── [    6445032]  participant02-test01-rep02-1g-5mins_normalised_quaternions.csv
├── [    6517790]  participant02-test02-rep01-1g-5mins_normalised_quaternions.csv
├── [    6509961]  participant02-test02-rep02-1g-5mins_normalised_quaternions.csv
├── [    6450256]  participant02-test03-rep01-1g-5mins_normalised_quaternions.csv
└── [    6439846]  participant02-test03-rep02-1g-5mins_normalised_quaternions.csv

0 directories, 12 files
```


## Demo dataset Thu-27-Jul-2023/
Create and go to data demo path
```
cd && mkdir -p $HOME/repositories/datasets/in2research2023 && cd $HOME/repositories/datasets/in2research2023
cd Thu-27-Jul-2023/
```
Tree of demo dataset. Each pair of video and time-series were recorded for approximately 60 seconds. 
```
$ cd $HOME/repositories/datasets/in2research2023/Thu-27-Jul-2023
$ tree -s
[       4096]  .
├── [       4096]  participant01
│   ├── [  114600268]  participant01-test01.avi
│   ├── [    1355571]  participant01-test01.avi.csv
│   ├── [  104162980]  participant01-test02.avi
│   ├── [    1226232]  participant01-test02.avi.csv
│   ├── [  109948016]  participant01-test03.avi
│   ├── [    1267036]  participant01-test03.avi.csv
│   ├── [  135885238]  participant01-test04.avi
│   ├── [    1606121]  participant01-test04.avi.csv
│   ├── [   89959140]  participant01-test05.avi
│   └── [    1478623]  participant01-test05.avi.csv
└── [       4096]  participant02
    ├── [  126647620]  participant02-test01.avi
    ├── [    1438384]  participant02-test01.avi.csv
    ├── [  112525428]  participant02-test02.avi
    ├── [    1260894]  participant02-test02.avi.csv
    ├── [  113749330]  participant02-test03.avi
    ├── [    1291345]  participant02-test03.avi.csv
    ├── [  122758162]  participant02-test04.avi
    ├── [    1331857]  participant02-test04.avi.csv
    ├── [  120859978]  participant02-test05.avi
    └── [    1318864]  participant02-test05.avi.csv

2 directories, 20 files
```

## Citations 
Hekim, Sujon, and Miguel Xochicale. ‘Demo Dataset for Multimodal Real-time Ai V.1.0.0’. Zenodo, 3 March 2024. https://doi.org/10.5281/zenodo.10775099.
