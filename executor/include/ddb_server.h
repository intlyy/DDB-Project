/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <fstream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "protos/ddb.grpc.pb.h"
#else
#include "ddb.grpc.pb.h"
#endif

#include "Tree.h"
#include "executor.h"
#include "myetcd.h"

#define BUFFER_SIZE 1024
#define SITE_NUMBER 2

using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientReader;
using grpc::ClientWriter;
using grpc::ServerWriter;
using ddb::CallRemoteDB;
using ddb::QueryNode;
using ddb::QueryTree;
using ddb::ExecuteNode;
using ddb::ExecuteTree;
using ddb::ResultPath;
using ddb::ResultSet;

typedef struct SITEINFO {
    string IP;
    int PORT;
}SITEINFO;

static SITEINFO sites[SITE_NUMBER] = {{"10.77.70.149", 54321}, {"10.77.70.150", 54321}};

void RunServer();
