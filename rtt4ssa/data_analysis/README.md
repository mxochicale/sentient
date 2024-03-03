# Motion-image sensor data analysis

## Running notebooks
```
cd $HOME/repositories/rtt4ssa/rtt4ssa/data_analysis
export PYTHONPATH="${PYTHONPATH}:$HOME/repositories/rtt4ssa"
mamba activate rtt4ssaVE 
jupyter notebook --browser=firefox
```

## Notebooks
You might like to run notebooks in the following order: 
* `A_preprocessing_data_from_multiple-files.py` or `A_preprocessing_data_from_multiple-files.ipynb` to create pre-processed data
* `B_analysis_of_pre-processed_data_from_multiple_files_plotting.ipynb` to analysis preprocessed data

## Notes
Notebooks might run out of memory to which is recommended to use python scripts [`]

## References 
[`] https://github.com/tensorflow/tensorflow/issues/9829#issuecomment-300783730 
