#include "DeleteStatement.h"

std::ostream& operator<<(std::ostream& os, DeleteStatement s) {
    os << std::string("delete_rname: ") << s.rname << std::string("\n");
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