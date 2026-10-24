# Examples for C++ API 

## Preparing building
Once openzen is built, run this to add OpenZenConfigTargets in OpenZenConfig.cmake
```bash
sed -i 's|OpenZenTargets\.cmake|OpenZenConfigTargets.cmake|' OpenZenConfig.cmake
```

## Files
* cpp scripts are part of OpenZen, under the MIT License.
* See https://bitbucket.org/lpresearch/openzen/src/master/LICENSE for details SPDX-License-Identifier: MIT
* CMake file to test the usage of a installed cmake package of OpenZen

## Build
```bash
cd ../openzen/cpp_examples
rm -rf build && cmake -S . -B build && cmake --build build -j$(nproc)
./build/example-cpp17
```

## References
https://bitbucket.org/lpresearch/openzen/src/master/standalone_example/  
https://lpresearch.bitbucket.io/openzen/latest/examples.html   
https://bitbucket.org/lpresearch/openzen/src/master/examples/  
https://bitbucket.org/lpresearch/openzen/src/master/standalone_example/  



