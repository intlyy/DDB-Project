#include "../include/myetcd.h"

etcd::Response query_etcd(etcd::Client *client, string key){
	return client->get(key).get();
}

etcd::Response set_etcd(etcd::Client *client, string key, string value){
	return client->set(key, value).get();
}

etcd::Response delete_etcd(etcd::Client *client, string key){
	return client->rm(key).get();
}

etcd::Response list_etcd(etcd::Client *client, string key){
	return client->ls(key).get();
}

etcd::Response list_etcd(etcd::Client *client, string key, size_t limit){
	return client->ls(key, limit).get();
}
