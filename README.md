<p float="left">
   <img src="docs/figures/logo.png" alt="sentient"/>
</p>

[![GitHub Discussions](https://img.shields.io/github/discussions/mxochicale/sentient)](https://github.com/mxochicale/sentient/discussions)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit)](https://github.com/pre-commit/pre-commit)

The aim of this repository is to develop S.E.N.T.I.E.N.T. library (Sensor Enhanced Network Technology Integrating Evolving Neural Tools).
`sentient` library includes multi-sensor data methods using SOTA (State-Of-The-Art) AI models for skills assessment in surgery, sports, and robotics.
Hence, this repository contains sentient's related material with dependencies, sample-data, scripts, unit tests, docs and references. 

Please refer to [discussion](https://github.com/mxochicale/sentient/discussions) for questions, ideas, polls, etc.
Feel free to open [issues](https://github.com/mxochicale/sentient/issues) instead.

## Getting started
You can run [notebooks](sentient/data_analysis) for data analysis.
See AI-enabled [models](sentient/models) (work in progress). 
If interested in cada collection, please see [sensor fusion](sentient/sensor_fusion) and [video_devices](sentient/video_devices).   

## Installation
```
conda create -n "sentientVE" python=3.10 pip
conda activate sentientVE
pip install --editable . # Install the package in editable mode
pip install .[test]
pip install .[learning]
#pip uninstall sentient
#conda deactivate
#conda remove -n sentientVE --all

```
## Pre-commmit
```
conda activate sentientVE
pre-commit run -a
```

### Testing sensor data
Just test default local camera id 0.   
```
cd $HOME/repositories/sentient
conda activate sentientVE
#export PYTHONPATH=$HOME/repositories/sentient
#export PYTHONPATH="${PYTHONPATH}:$HOME/repositories/sentient"
python -m pytest -v -s tests/
python -m pytest -v -s tests/test_video_capture.py::test_simple_list_of_available_video_devices
python -m pytest -v -s tests/test_video_capture.py::test_capture_video
```

## Notebooks
```
conda activate sentientVE
cd $HOME/repositories/sentient/notebooks
#export PYTHONPATH="${PYTHONPATH}:$HOME/repositories/sentient"
jupyter notebook --browser=firefox
```

## Clone repository
* Generate your SSH keys as suggested [here](https://docs.github.com/en/github/authenticating-to-github/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) (or [here](https://github.com/mxochicale/tools/blob/main/github/SSH.md))
* Clone the repository by typing (or copying) the following line in a terminal at your selected path in your machine:
```
cd && mkdir -p $HOME/repositories && cd  $HOME/repositories
git clone git@github.com:mxochicale/sentient.git
```

## Publications
```
@misc{leung-xochicale-rami-icra2023,
      author={
            Tsz Yan Leung and 
            Miguel Xochicale},
      title={
            Towards a Simple Framework of Skill Transfer Learning for 
            Robotic Ultrasound-guidance Procedures}, 
      year={2023},
      eprint={2305.04004},
      archivePrefix={arXiv},
      primaryClass={cs.RO},
      howpublished = "\url{https://arxiv.org/abs/2305.04004}"},
      url = {https://github.com/mxochicale/rami-icra2023}
}
``` 

## Contributors
Thanks goes to all these people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):  
<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
	<!-- CONTRIBUTOR -->
	<td align="center">
		<!-- ADD GITHUB USERNAME AND HASH FOR GITHUB PHOTO -->
		<a href="https://github.com/???"><img src="https://avatars1.githubusercontent.com/u/23114020?v=4?s=100" width="100px;" alt=""/>
		<br />
			<sub> <b>ADD NAME SURNAME</b> </sub>        
		</a>
		<br />
			<!-- ADD GITHUB REPOSITORY AND PROJECT, TITLE AND EMOJIS -->
			<a href="https://github.com/$PROJECTNAME/$REPOSITORY_NAME/commits?author=" title="Research">  🔬 🤔  </a>
	</td>
	<!-- CONTRIBUTOR -->
	<td align="center">
		<!-- ADD GITHUB USERNAME AND HASH FOR GITHUB PHOTO -->
		<a href="https://github.com/AbuAbdul1ah"><img src="https://avatars1.githubusercontent.com/u/131908567?v=4?s=100" width="100px;" alt=""/>
		<br />
			<sub> <b>Sujon Hekim</b> </sub>        
		</a>
		<br />
			<!-- ADD GITHUB REPOSITORY AND PROJECT, TITLE AND EMOJIS -->
			<a href="https://github.com/mxochicale/in2research2023/commits?author=AbuAbdul1ah" title="Research">  🔬 🤔  </a>
	</td>
	<!-- CONTRIBUTOR -->
	<td align="center">
		<a href="https://github.com/mxochicale"><img src="https://avatars1.githubusercontent.com/u/11370681?v=4?s=100" width="100px;" alt=""/>
			<br />
			<sub><b>Miguel Xochicale</b></sub>          
			<br />
		</a>
			<a href="https://github.com/mxochicale/in2research2023/commits?author=mxochicale" title="Code">💻</a> 
			<a href="ttps://github.com/mxochicale/in2research2023/commits?author=mxochicale" title="Documentation">📖  🔧 </a>
	</td>
  </tr>
</table>
<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

This work follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification.  
Contributions of any kind welcome!
