#include <stdio.h>
#include <mysql/mysql.h>
#include <string>
#include <cstring>
#include <vector>
#include<cassert>
#include<iostream>

#define HOST "localhost"
#define USERNAME "root"
#define PASSWORD "Ruc@mysql"
//#define DATABASE "test"  
//#define PORT 7654 
#define TMPPATH "/mnt/d/DDB/"
#define SOURCEPATH "/mnt/d/DDB/"

using namespace std;

auto Insert(string sql,int site) ->string;

auto Delete(string sql,int site) ->string;

auto Select(string sql,int site,string res_name) ->string;

auto Load(string sql_create,string sql_load,int site) ->string;

auto tmp_load(string tmp_data,int site) ->string;

auto res_print(string res_name) ->void;

auto res_getrows(string res_name) ->int;

auto Mysql_Delete(string sql, int site) ->string;

vector<int> res_getcol1(string res_name); 

