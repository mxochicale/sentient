# Examples for C++ API 

## Preparing building
Once openzen is built, please add the following line in `$HOME/repositories/openzen/build/OpenZenConfig.cmake`
```
include("${CMAKE_CURRENT_LIST_DIR}/OpenZenConfigTargets.cmake")
```

## Files
* cpp scripts are part of OpenZen, under the MIT License.
* See https://bitbucket.org/lpresearch/openzen/src/master/LICENSE for details SPDX-License-Identifier: MIT
* CMake file to test the usage of a installed cmake package of OpenZen

## Build
```
cd $HOME/repositories/sentient/dependencies/openzen/cpp_examples && mkdir -p build && cd build
cd .. && rm -rf build && mkdir build && cd build && cmake .. && cmake --build .
./example-cpp* 
```

## References
https://bitbucket.org/lpresearch/openzen/src/master/standalone_example/  
https://lpresearch.bitbucket.io/openzen/latest/examples.html   
https://bitbucket.org/lpresearch/openzen/src/master/examples/  
https://bitbucket.org/lpresearch/openzen/src/master/standalone_example/  



