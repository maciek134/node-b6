cmake_minimum_required(VERSION 3.1)
project(node_b6 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules/)

include(NodeJS.cmake)
nodejs_init()

add_nodejs_module(${PROJECT_NAME} src/addon.cc src/DeviceWrap.cc)

find_package(libusb-1.0)
if (LIBUSB_1_FOUND)
    include_directories(${LIBUSB_1_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${LIBUSB_1_LIBRARIES})
endif(LIBUSB_1_FOUND)

target_link_libraries(${PROJECT_NAME} b6)