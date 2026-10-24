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
./build/example-cpp17 --verbose 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # --verbose
./build/example-cpp17 00:04:3E:53:ED:5B 100 921600 # custom MAC + rate + baud # just dots per sample
```


## Logs

```
--- Timing summary (wall clock) ---
Samples:               100
Elapsed:               9700.53 ms
Observed rate:         10.2056 Hz  (requested 10 Hz)
Mean interval:         97.9852 ms  (10.2056 Hz)
Min / Max interval:    0.003247 / 124.947 ms
RMS jitter:            15.5992 ms
Sensor connection closed
Received 100 IMU samples
```

Observed rate is the most trustworthy number: it uses the total wall-clock time across all N-1 inter-sample intervals, so it's not thrown off by a single long stall.

Mean interval should be ≈ 1000 / samplingRate ms (e.g. 20 ms at 50 Hz).

RMS jitter tells you how consistent the arrival is. With Bluetooth and an OS scheduler in the loop, expect a few ms of jitter even if the sensor hardware is precise. Occasional large spikes in Max interval are common — look at the histogram if you want detail.

If Observed rate is systematically lower than requested, the sensor is likely dropping packets or the link (Bluetooth) can't keep up. If it's higher, the sensor may be ignoring your requested rate and running at its native rate.


## References
https://bitbucket.org/lpresearch/openzen/src/master/standalone_example/  
https://lpresearch.bitbucket.io/openzen/latest/examples.html   
https://bitbucket.org/lpresearch/openzen/src/master/examples/  
https://bitbucket.org/lpresearch/openzen/src/master/standalone_example/  



