### build preparation
1. clone repository
2. download LibTorch library from: https://pytorch.org/  choose cxx11 ABI version for your OS
3. enter project root folder:
```
    cd simple-traffic-lights-component 
```
5. create libraries folder:
```
    mkdir libraries
```
6. unzip pytorch to libraries folder:
```
    uzip libtorch_lib_archive.zip -d path_to_libraries_folder
```
7. download latest nlohmann json release from: https://github.com/nlohmann/json/releases
8. uznip it to libraries folder:
```
    unzip include.zip -d path_to_libraries_folder
```
9. install SimpleAmqpClient library:
    1. install pre-requisites:
        1. install [boost](https://www.boost.org/users/history/version_1_73_0.html)
        2. install [rabbitmq-c](https://github.com/alanxz/rabbitmq-c)
    2. download latest SimpleAmqpClient release from: https://github.com/alanxz/SimpleAmqpClient/releases
    3. unzip archive to some folder
    4. install SimpleAmqpClient to standard path following Build procedure:
```
    mkdir simpleamqpclient-build
    cd simpleamqpclient-build
    cmake ..
    make install
```
### building
1. create build directory in simple-traffic-lights-component folder
```
    mkdir build
```
2. use cmake to generate makefiles:
```
    cmake -DCMAKE_PREFIX_PATH=../libraries/libtorch ..
```
3. use cmake to build project:
```
    cmake --build Release . --config Release
```
### running
1. execute library with valid json config file:
```
    ./simple-traffic-lights-component ptah_to_config.json
```
### json configuration file structure
```
    {
        "broker-address" : String,
        "vhost-name": String,
        "username": String,
        "password": String,
        "requests-queue": String,
        "model-path": String,
        "input-size": Integer
    }
```
