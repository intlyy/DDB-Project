#include <vector>
#include <string>
#include <iostream>

#include "SelectStatement.h"

SelectStatement::SelectStatement() {}

void SelectStatement::add_from(std::string table_name) {
    this->from.push_back(table_name);
}

void SelectStatement::add_select(std::string attribute_name) {
    this->select.push_back(attribute_name);
}

void SelectStatement::add_where(Predicate predicate) {
    this->where.push_back(predicate);
}

std::ostream& operator<<(std::ostream& os, SelectStatement s) {
    os << std::string("select_list: ");
    for(auto attribute : s.select) {
        os << attribute << std::string("\t");
    }
    os << std::string("\n");
    os << std::string("from_list: ");
    for(auto rname : s.from) {
        os << rname << std::string("\t");
    }
    os << std::string("\n");
    if(s.where.size() > 0) {
        os << std::string("where_list: ");
        for(int i=0; i<s.where.size(); ++i) {
            if(i > 0) os << std::string(" AND ");
            os << s.where[i];
        }
        os << std::string("\n");
    }
    return os;
}