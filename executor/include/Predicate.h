# ifndef PREDICATE_H
# define PREDICATE_H
#include <string>
enum PredicateType{filter = 1, join = 2};
class Predicate{
    private:
    friend std::ostream& operator<<(std::ostream& os, Predicate p){
        os << p.left_name << p.op_type << p.right_name;
        return  os;
    }
    public:
    std::string op_type;
    PredicateType pre_type;    //1->filter  2->join
    std::string left_name;
    std::string right_name;
    Predicate(PredicateType pre_type, std::string left_name, std::string op_type, std::string right_name):
    pre_type(pre_type),left_name(left_name),op_type(op_type),right_name(right_name){}
    std::string to_string() const{
        return left_name + op_type + right_name;
    }
};
#endif