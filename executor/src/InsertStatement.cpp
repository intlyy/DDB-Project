#include "InsertStatement.h"

InsertStatement::InsertStatement() {
    this->rname = "";
    this->values = "";
}

void InsertStatement::add_value(double value) {
    if(this->values != "") {
        this->values += ", ";
    }
    std::ostringstream myos;
    myos << value;
    std::string result = myos.str();
    this->values += result;
}

void InsertStatement::add_value(int value) {
    if(this->values != "") {
        this->values += ", ";
    }
    this->values += std::to_string(value);
}

void InsertStatement::add_value(std::string value) {
    if(this->values != "") {
        this->values += ", ";
    }
    this->values = this->values + "'" + value + "'";
}

std::ostream& operator<<(std::ostream& os, InsertStatement s) {
    os << std::string("insert_rname: ") << s.rname  << std::string("\n");
    os << std::string("insert_values: ") << s.values << std::string("\n");
    return os;
}