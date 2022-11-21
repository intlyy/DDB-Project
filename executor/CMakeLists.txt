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

cmake_minimum_required(VERSION 3.5.1)

project(ddb C CXX)

include(common.cmake)

# Proto file
get_filename_component(hw_proto "protos/ddb.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/ddb.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/ddb.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/ddb.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/ddb.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# include absl
include_directories($HOME/grpc/include)
include_directories($HOME/.local/include)
include_directories(/usr/local/include/etcd)


include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
file(GLOB SRC_LIST "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
add_executable(main ${SRC_LIST})
add_executable(server_main src/server_main.cc src/ddb_server.cpp src/executor.cpp src/Tree.cpp src/connect2sql.cpp src/myetcd.cpp)
target_link_libraries(main
    hw_grpc_proto
    mysqlclient
    etcd-cpp-api
    cpprest
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(server_main
    hw_grpc_proto
    mysqlclient
    etcd-cpp-api
    cpprest
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

set(CMAKE_CXX_FLAGS -pthread)

