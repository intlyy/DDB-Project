#ifndef METADATA_H
#define METADATA_H
#include <set>
#include <map>
#include <iostream>  
#include <string>
#include <map> 
#include <regex>
#include <vector>
#include <sstream>
#include <typeinfo>
# include "myetcd.h"
#include "utils.h"
using namespace std;
// fragment
struct FragDef {
	int id;
    int site;
    vector<string> column; //该分片涉及到的columns
	vector<string> condition; //分片条件
    int size; //记录该分片的大小，以byte为单位
};
// table-fragment
struct Fragment {
	string name;
	string fragtype; // H/V 
	int fragnum;
    set<string> fragcolumns;  //该表涉及分片的列
	vector<FragDef> frags;
};
struct ColumnDef{
    string name;
    string type;
    string desc;
    bool key;
};
struct RelationDef{
    string name;
    vector<ColumnDef> cols;
};
// 每条查询的表结构
typedef struct QueryTable{
    string name;
    set<string> column_names;
}QueryTable;
class ZBase64{
private:
    //Base64编码解码表
    char* m_Base64_Table;
public:
    //构造
    ZBase64();
    //编码
    string EncodeBase64(const string strSource);
    //解码
    string DecodeBase64(const string strSource);
};
vector<string> split2list(string info);
vector<string> getTables();
vector<string> getTableAttri(etcd::Client* client,string tableName);
string getTableAttriType(etcd::Client* client,string tableName, string attriName);
string getTableAttriDesc(etcd::Client* client,string tableName, string attriName);
string getTableKey(string tableName);
string getTableFragType(etcd::Client* client,string tableName);
vector<string> getTableFragH(etcd::Client* client,string tableName);
int getTableFragNum(etcd::Client* client,string tableName);
string getTableFragCondition(etcd::Client* client,string tableName, int index);
string getTableFragCol(etcd::Client* client,string tableName, int index);
int getTableFragSize(etcd::Client* client,string tableName,int index);
int getTableFragSite(etcd::Client* client,string tableName,int index);
bool saveTableToEtcd(RelationDef table);
RelationDef getTableFromEtcd(string tablename);
bool saveFragToEtcd(Fragment frag);
Fragment getFragFromEtcd(string tablename);
bool Insert_Attrvalue(etcd::Client* client,string &key,string &value);
map<string,Fragment> GetETCDFragment();
#endif
