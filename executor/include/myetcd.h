#include <etcd/Client.hpp>
#include <etcd/Response.hpp>
#include <string>

using namespace std;
const std::string ENDPOINTS = "http://10.77.70.149:2379, http://10.77.70.150:2379";
const std::string FILE_PATH_KEY = "/sites/file_path/";

etcd::Response query_etcd(etcd::Client *client, string key);
etcd::Response set_etcd(etcd::Client *client, string key, string value);
etcd::Response delete_etcd(etcd::Client *client, string key);
etcd::Response list_etcd(etcd::Client *client, string key, size_t limit);
etcd::Response list_etcd(etcd::Client *client, string key);
