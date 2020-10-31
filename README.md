### building and running
1. clone repository
2. download LibTorch library from: https://pytorch.org/ (choose correct OS, note: Debug libraries doesn't work on windows)
3. enter project root folder:
```
    cd simple-traffic-lights-component 
```
4. create folder build and enter it:
```
    mkdir build && cd build
```
5. create makefiles:
```
    cmake -DCMAKE_PREFIX_PATH=/path/to/libtorch ..
```
6. build target:
```
    cmake --build . --config Release
```
7. execute binary file:
```
    ./simple-traffic-lights-component
```

