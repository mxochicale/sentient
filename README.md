![logo](docs/figures/logo.png)

We are interested in prototyping real-time transformer-based models for surgical skills assessment (rtt4ssa).
This repository hence contains rtt4ssa's related material with dependencies, sample-data, scripts, unit tests, and few docs and references. 

Please refer to [discussion](https://github.com/mxochicale/rtt4ssa/discussions/landing) for questions, ideas, polls, etc.
Feel free to open [issues](https://github.com/mxochicale/rtt4ssa/issues) instead.

## Usage

### Testing sensor data
Just test default local camera id 0.   
```
mamba activate rtt4ssaVE
export PYTHONPATH=$HOME/repositories/rtt4ssa
python -m pytest -v -s tests/
python -m pytest -v -s tests/test_video_capture.py::test_simple_recording_video
python -m pytest -v -s tests/test_video_capture.py::test_capture_video
```

## Clone repository
* Generate your SSH keys as suggested [here](https://docs.github.com/en/github/authenticating-to-github/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) (or [here](https://github.com/mxochicale/tools/blob/main/github/SSH.md))
* Clone the repository by typing (or copying) the following line in a terminal at your selected path in your machine:
```
cd && mkdir -p $HOME/repositories && cd  $HOME/repositories
git clone git@github.com:mxochicale/rtt4ssa.git
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
