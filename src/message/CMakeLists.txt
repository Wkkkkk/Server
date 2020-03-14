# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ helloworld example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building helloworld.

cmake_minimum_required(VERSION 3.5)

project(message C CXX)
set(MESSAGE_PROTO test)
file(GLOB_RECURSE SOURCES_CXX *.cpp *.h)

set(hw_proto_srcs_list "")
set(hw_grpc_srcs_list "")

foreach (message_name ${MESSAGE_PROTO})
    # Proto file
    get_filename_component(hw_proto "./proto/${message_name}.proto" ABSOLUTE)
    get_filename_component(hw_proto_path "${hw_proto}" PATH)
    message(STATUS "Found proto ${hw_proto}")

    # Generated sources
    set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/${message_name}.pb.cc")
    set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${message_name}.pb.h")
    set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/${message_name}.grpc.pb.cc")
    set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${message_name}.grpc.pb.h")
    add_custom_command(
            OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
            COMMAND ${_PROTOBUF_PROTOC}
            ARGS --grpc_out "generate_mock_code=true:${CMAKE_CURRENT_BINARY_DIR}"
            --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
            -I "${hw_proto_path}" -I "${Protobuf_INCLUDE_DIRS}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${hw_proto}"
            DEPENDS "${hw_proto}")

    list(APPEND hw_proto_srcs_list "${hw_proto_srcs}")
    list(APPEND hw_grpc_srcs_list "${hw_grpc_srcs}")

endforeach (message_name)

add_library(message
        ${SOURCES_CXX}
        ${hw_proto_srcs_list}
        ${hw_grpc_srcs_list}
        )

target_link_libraries(message
        ${AsyncGRPC_LIBRARIES}
        ${_GRPC_GRPCPP_UNSECURE}
        ${_PROTOBUF_LIBPROTOBUF}
        ${GTEST_LIBRARIES}
        glog::glog
        )
