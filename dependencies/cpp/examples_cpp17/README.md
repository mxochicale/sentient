# Examples

## Build and run it
step 1: configure: -S . Source directory. #-B build Build directory. #Define a cache variable.
step 2: compile: invokes the compiler (via make/ninja) on helloworld.cpp and links the helloworld binary
```bash
rm -rf build && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release 
cmake --build build -j$(nproc)
./build/hiworld
./build/hiargs foo bar
./build/hello_cpp17
```
* clean
```bash
cmake --build build --target clean
rm -rf build
```


