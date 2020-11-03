### building and running
1. clone repository
2. download LibTorch library from: https://pytorch.org/ (choose correct OS, note: Debug libraries doesn't work on windows)
3. enter project root folder:
```
    cd simple-traffic-lights-component 
```
5. create libraries folder:
```
    mkdir libraries
```
6. install [redboltz/mqtt_cpp](https://github.com/nlohmann/json) to libraries folder:
```
    cd libraries && git clone https://github.com/redboltz/mqtt_cpp.git
```
7. install [nlohmann/json](https://github.com/redboltz/mqtt_cpp) to libraries folder:
    Download and uznip include.zip from latest release of nlohmann/json.
8. create build folder and enter it:
```
    mkdir build && cd build
```
9. create makefiles:
```
    cmake -DCMAKE_PREFIX_PATH=/path/to/libtorch ..
```
10. build target:
```
    cmake --build . --config Release
```
11. execute binary file:
```
    ./simple-traffic-lights-component model_name mqtt_broker_address
```

