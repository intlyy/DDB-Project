#include "../include/myetcd.h"

void init_etcd(int site_number){
	etcd::Client client(ENDPOINTS);
	for(int i = 1; i <= site_number; i++){
	    set_etcd(&client, FILE_PATH_KEY+std::to_string(i), "/mnt/d/DDB/");
	}
}

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
