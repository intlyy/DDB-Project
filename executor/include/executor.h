#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include "connect2sql.h"
#include "Tree.h"
#include <thread>
#include <future>

#define LOCALSITE  1
#define LOCALSITE2  4 
#define MAXTHREAD 4

ETree Data_Select_Execute(QTree tree);

void Data_Select_Execute_thread(QTree tree, std::promise<ETree> &resultObj);

void RPC_Data_Select_Execute_Thread(QTree tree, int site, std::promise<ETree> &resultObj);

string Data_Insert_Delete_Execute(vector<int> sitenames, vector<string> sqls);

void Data_Insert_Delete_Thread(int site, string frag_sql, std::promise<string> &resultObj);

string RPC_Insert(int site, string sql);

string Data_Load_Execute(string create_sql, string load_sql, string main_name, vector<int> sitenames, vector<string> sqls, vector<string> table_names);

void Data_Load_Thread(int site, string frag_sql, string frag_name, std::promise<string> &resultObj);

string RPC_Local_Tmp_Load(string localname, int site);
