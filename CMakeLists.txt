cmake_minimum_required(VERSION 3.16)
project(simple-traffic-lights-component)

set(CMAKE_CXX_STANDARD 14)

find_package(Torch REQUIRED)

find_library(RABBITMQCPP SimpleAmqpClient)

include_directories(${PROJECT_SOURCE_DIR}/libraries/include)
include_directories("/usr/local/include/")

add_executable(
    ${PROJECT_NAME} 
    src/main.cpp
    src/model/Model.cpp
    src/controller/TLSController.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}
    ${TORCH_LIBRARIES}
    ${RABBITMQCPP}
    pthread
    )