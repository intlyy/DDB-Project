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

#include "../include/ddb_server.h"

class DDBRemoteCallClient {
 public:
  DDBRemoteCallClient(std::shared_ptr<Channel> channel)
      : stub_(CallRemoteDB::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  ETree RemoteSelect (QTree request) {
    ClientContext context;
    QueryTree real_req;
    ExecuteTree real_reply;
    real_req.set_tree_id(request.tree_id);
    real_req.set_root(request.root);
    for(int i = 0; i < request.Nodes.size(); i++) {
      QTNode qtnode = request.Nodes[i];
      QueryNode *qn = real_req.add_nodes();
      qn->set_id(qtnode.id);
      for( int s = 0; s < qtnode.children.size(); s++)
        qn->add_children(qtnode.children[s]);
      qn->set_parent(qtnode.parent);
      qn->set_sql_statement(qtnode.sql_statement);
      qn->set_site(qtnode.site);
    }

    Status status = stub_->RemoteSelect(&context, real_req, &real_reply);

    ETree reply;
    reply.tree_id = real_reply.tree_id();
    reply.root = real_reply.root();
    for (int j = 0; j < real_reply.nodes_size(); j++ ) {
      ETNode etnode;
      ExecuteNode en = real_reply.nodes(j);
      etnode.id = en.id();
      etnode.vol = en.vol();
      etnode.res = en.res();
      for(int k = 0; k < en.children_size(); k++)
        etnode.children.push_back(en.children(k));
      etnode.parent = en.parent();
      etnode.site = en.site();
      etnode.time_spend = en.time_spend();
      reply.Nodes.push_back(etnode);
    }

    // Act upon its status.
    if (status.ok()) {
	    std::cout << "receive finished.";
	    return reply;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      std::cout << "RPC failed";
      return reply;
    }
  }

  std::string RemotePushFile (ResultPath &request) {
    ClientContext context;
    ResultPath path;
    std::unique_ptr<ClientWriter<UploadSet>> writer(stub_->RemotePushFile(&context, &path));
    UploadSet us;
    char buffer[BUFFER_SIZE];
    string filename = request.src_path();
    std::ifstream infile;
    infile.open(filename, std::ifstream::in | std::ifstream::binary);
    while (!infile.eof()) {
      infile.read(buffer, BUFFER_SIZE);
      us.set_data(buffer, infile.gcount());
      us.set_filename(filename);
      if(!writer->Write(us)) break;
    }
    infile.close();
    writer->WritesDone();
    Status status = writer->Finish();
    // Act upon its status.
    if (status.ok()) {
	    return "OK";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "FAIL";
    }
  }

  std::string RemoteFetchFile (ResultPath &request) {
    ClientContext context;
    std::unique_ptr<ClientReader<ResultSet>> reader(stub_->RemoteFetchFile(&context, request));
    ResultSet rs;
    string filename = request.target_path();
    std::ofstream outfile;
    const char *data;
    outfile.open(filename, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    while (reader->Read(&rs)){
      data = rs.data().c_str();
      outfile.write(data, rs.data().length());
    }
    Status status = reader->Finish();
    outfile.close();
    // Act upon its status.
    if (status.ok()) {
	    return "OK";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "FAIL";
    }
  }

  std::string RemoteInsert (Statement stat) {
    std::cout << "client receive insert req, site: " << stat.site() << ", sql: " << stat.sql() << std::endl;
    ClientContext context;
    StatementResult stat_res;

    Status status = stub_->RemoteInsert(&context, stat, &stat_res);

    // Act upon its status.
    std::cout << "status: " << status.ok() <<std::endl;
    if (status.ok()) {
	    std::cout << "receive finished.";
	    return "OK";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      std::cout << "RPC failed";
      return "FAIL";
    }
  }

  std::string RemoteTmpLoad (Statement stat) {
    ClientContext context;
    StatementResult stat_res;

    Status status = stub_->RemoteTmpLoad (&context, stat, &stat_res);

    // Act upon its status.
    if (status.ok()) {
	    std::cout << "receive finished.";
	    return "OK";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      std::cout << "RPC failed";
      return "FAIL";
    }
  }
 private:
  std::unique_ptr<CallRemoteDB::Stub> stub_;
};

string RPC_Insert(int site, string frag_sql)
{
    string target_str = sites[site-1].IP + ":" + std::to_string(sites[site-1].PORT);
    std::cout << "rpc target is: " << target_str <<std::endl;
    DDBRemoteCallClient ddbclient(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    Statement stat;
    stat.set_site(site);
    stat.set_sql(frag_sql);
    string execute_result = ddbclient.RemoteInsert(stat);
    return execute_result;   
}

string RPC_Local_Tmp_Load(string localname, int site){
    string target_str = sites[site-1].IP + ":" + std::to_string(sites[site-1].PORT);
    std::cout << "rpc target is: " << target_str <<std::endl;
    DDBRemoteCallClient ddbclient(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    etcd::Client etcd_client(ENDPOINTS);
    string SourcePath = query_etcd(&etcd_client, FILE_PATH_KEY+to_string(site)).value().as_string();
    string LocalPath = query_etcd(&etcd_client, FILE_PATH_KEY+to_string(LOCALSITE)).value().as_string();
    if ( SourcePath.empty() || LocalPath.empty()) {
      cout << "File store path not been set in site " << site <<endl;
      return "";
    }
    SourcePath += localname + ".sql";
    LocalPath += localname + ".sql";
    cout << "source path from etcd is: " << SourcePath <<endl;
    cout << "local path from etcd is: " << LocalPath <<endl;
    ResultPath rs;
    rs.set_src_path(SourcePath);
    rs.set_target_path(LocalPath);
    string fetch_result = ddbclient.RemotePushFile(rs);
    if (fetch_result == "OK") {
      cout << localname << "transfered success!" << endl;
    } else {
      cout << localname << "transfered failed!" << endl;
    }
    Statement stat;
    stat.set_site(site);
    stat.set_sql(localname);
    string execute_result = ddbclient.RemoteTmpLoad(stat);
    return execute_result;
}

void RPC_Data_Select_Execute_Thread(QTree qtree, int site, std::promise<ETree> &resultObj){
    string target_str = sites[site-1].IP + ":" + std::to_string(sites[site-1].PORT);
    std::cout << "rpc target is: " << target_str <<std::endl;
    DDBRemoteCallClient ddbclient(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    ETree execute_result = ddbclient.RemoteSelect(qtree);
    string res_name = "tree_" + to_string(qtree.tree_id) + "_node_" + to_string(qtree.root);
    etcd::Client etcd_client(ENDPOINTS);
    string SourcePath = query_etcd(&etcd_client, FILE_PATH_KEY+to_string(site)).value().as_string();
    string LocalPath = query_etcd(&etcd_client, FILE_PATH_KEY+to_string(LOCALSITE)).value().as_string();
    if ( SourcePath.empty() || LocalPath.empty()) {
      cout << "File store path not been set in site " << site <<endl;
      return;
    }
    SourcePath += res_name + ".sql";
    LocalPath += res_name + ".sql";
    cout << "source path from etcd is: " << SourcePath <<endl;
    cout << "local path from etcd is: " << LocalPath <<endl;
    ResultPath rs;
    rs.set_src_path(SourcePath);
    rs.set_target_path(LocalPath);
    string fetch_result = ddbclient.RemoteFetchFile(rs);
    if (fetch_result == "OK") {
      cout << res_name << "transfered success!" << endl;
    } else {
      cout << res_name << "transfered failed!" << endl;
    }
    resultObj.set_value(execute_result);
}

void output_query_node(QTNode node){
	std::cout << "node id: " << node.id <<std::endl;
	std::cout << "node children: ";
	std::copy(node.children.begin(), node.children.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl << "node parent: " << node.parent << std::endl;
	std::cout << "node sql_statement: " << node.sql_statement  <<std::endl;
	std::cout << "node site: " << node.site  <<std::endl;
}

void output_execute_node(ETNode node){
	std::cout << "node id: " << node.id <<std::endl;
	std::cout << "node vol: " << node.vol <<std::endl;
	std::cout << "node res: " << node.res <<std::endl;
	std::cout << "node children: ";
	std::copy(node.children.begin(), node.children.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl << "node parent: " << node.parent << std::endl;
	std::cout << "node site: " << node.site  <<std::endl;
	std::cout << "node time_spend: " << node.time_spend <<std::endl;
}


class DDBCallRemoteCallServiceImpl final : public CallRemoteDB::Service {
  Status RemoteSelect(ServerContext* context, const QueryTree* request,
                  ExecuteTree* reply) override {
    QTree *real_req = new QTree;
    real_req->tree_id = request->tree_id();
    real_req->root = request->root();
    for(int i = 0; i < request->nodes_size(); i++) {
      QueryNode qnode = request->nodes(i);
      QTNode *qtn = new QTNode;
      qtn->id = qnode.id();
      for( int s = 0; s < qnode.children_size(); s++)
        qtn->children.push_back(qnode.children(s));
      qtn->parent = qnode.parent();
      qtn->sql_statement = qnode.sql_statement();
      qtn->site = qnode.site();
      real_req->Nodes.push_back(*qtn);
      //output_query_node(*qtn);
      free(qtn);
    }
    
    
    ETree real_reply = Data_Select_Execute(*real_req);
    free(real_req);
    reply->set_tree_id(real_reply.tree_id);
    reply->set_root(real_reply.root);
    for (int j = 0; j < real_reply.Nodes.size(); j++ ) {
      ETNode etnode = real_reply.Nodes[j];
      ExecuteNode *en = reply->add_nodes();
      en->set_id(etnode.id);
      en->set_vol(etnode.vol);
      en->set_res(etnode.res);
      for(int k = 0; k < etnode.children.size(); k++)
        en->add_children(etnode.children[k]);
      en->set_parent(etnode.parent);
      en->set_site(etnode.site);
      en->set_time_spend(etnode.time_spend);
      //output_execute_node(etnode);
    }
    cout << "Site " << LOCALSITE <<" has handled remote request finish!" <<endl;
    return Status::OK;
  }

  
  Status RemotePushFile(ServerContext* context, ServerReader<UploadSet>* reader, ResultPath* reply) override {
    UploadSet us;
    const char *data;
    reader->Read(&us);
    data = us.data().c_str();
    string filename = us.filename();
    std::ofstream outfile;
    outfile.open(filename, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    outfile.write(data, us.data().length());
    while (reader->Read(&us)){
      data = us.data().c_str();
      outfile.write(data, us.data().length());
    }
    outfile.close();
    std::cout << "server handle remote push file call." << std::endl;
    return Status::OK;
  }

  Status RemoteFetchFile(ServerContext* context, const ResultPath* request,
                  ServerWriter<ResultSet>* writer) override {
    ResultSet rs;
    char buffer[BUFFER_SIZE];
    string filename = request->src_path();
    std::ifstream infile;
    infile.open(filename, std::ifstream::in | std::ifstream::binary);
    while (!infile.eof()) {
      infile.read(buffer, BUFFER_SIZE);
      rs.set_data(buffer, infile.gcount());
      if(!writer->Write(rs)) break;
    }
    infile.close();
    std::cout << "Handle remote fetch file call." << std::endl;
    return Status::OK;
  }

  Status RemoteInsert (ServerContext* context, const Statement* request,
                  StatementResult* reply) override {
    string res, sql;
    int site;
    site = request->site();
    sql = request->sql();
    cout << "remote site receive insert req, site: " << site << ", sql: " <<sql <<endl;
    res = Insert(sql, site);
    cout << "remote insert finish. result: " << res << endl;

    reply->set_result(res);
    cout << "Site " << LOCALSITE <<" has handled remote insert request finish!" <<endl;
    return Status::OK;
  }

  Status RemoteTmpLoad (ServerContext* context, const Statement* request,
                  StatementResult* reply) override {
    string res;
    res = tmp_load(request->sql(), request->site());

    reply->set_result(res);
    cout << "Site " << LOCALSITE <<" has handled remote load request finish!" <<endl;
    return Status::OK;
  }
};


void RunServer() {
  init_etcd(SITE_NUMBER);
  std::string server_address("0.0.0.0:54321");
  DDBCallRemoteCallServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

