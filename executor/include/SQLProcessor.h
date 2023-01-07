#ifndef SQLPROCESSOR_H
#define SQLPROCESSOR_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "/home/liuwei/sql-parser-master/src/SQLParser.h"
#include "/home/liuwei/sql-parser-master/src/util/sqlhelper.h"
#include "SelectStatement.h"
#include "InsertStatement.h"
#include "DeleteStatement.h"
#include "ImportStatement.h"
#include "metadata.h"
#include "utils.h"
class SQLProcessor {
private:
    hsql::SelectStatement* select_stat;
    hsql::InsertStatement* insert_stat;
    hsql::DeleteStatement* delete_stat;
    hsql::CreateStatement* create_stat;
    hsql::ImportStatement* import_stat;
    hsql::SQLParserResult result;
    hsql::SQLStatement* stat;
    void solve_expr(hsql::Expr* expr, std::vector<Predicate>& where);  // solve the expr recursively
    std::string get_aname_from_expr(hsql::Expr* expr);
    Fragment GetFragmentFromCreateFragment();
public:
    std::string query;
    SelectStatement select;
    InsertStatement insert;
    DeleteStatement delete_s;
    RelationDef create_table;
    ImportStatement import_s;
    string createfrag_name;
    int sql_type;   // 1:select 2:insert 3.delete 4.create table
    bool is_valid;    
    SQLProcessor(std::string query);
    inline std::string double2str(double number){
        std::ostringstream myos;
        myos << number;
        std::string result = myos.str();
        return result;
    }
    inline std::string lower_string(std::string s) {
        int len = s.size();
        for(int i = 0; i < len; ++i){
            if(s[i] >= 'A' && s[i] <= 'Z'){
                s[i] += 32;
            }
        }
        return s;
    }
};

#endif