cmake_minimum_required(VERSION 3.5)
project(TrafficInfoDriver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to Release")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(COPY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
set(Boost_USE_STATIC_LIBS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(gflags REQUIRED)
find_package(glog 0.3.5 REQUIRED)
find_package(Boost COMPONENTS thread date_time iostreams serialization chrono atomic filesystem system log REQUIRED)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(c-ares CONFIG)
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
message(STATUS "_GRPC_CPP_PLUGIN_EXECUTABLE: ${_GRPC_CPP_PLUGIN_EXECUTABLE}")

find_package(AsyncGRPC REQUIRED)
message(STATUS "Using AsyncGRPC")

find_package(GTest REQUIRED)
message(STATUS "Using GTest")

include_directories(
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${GTEST_INCLUDE_DIRS}
)
link_directories(
        ${Boost_LIBRARY_DIRS}
)

add_subdirectory(src)