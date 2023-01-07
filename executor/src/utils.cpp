#include "../include/utils.h"
#include <iostream>
pair<string,string> getsplit(string name, const char word){
    int pos = name.find_first_of(word);
    pair<string,string> temp;
    if(pos != string::npos){
        temp = make_pair(name.substr(0, pos), name.substr(pos+1, name.size()));
        return temp;
    }
    else{
        temp = make_pair("ERROR","ERROR");
        return temp;
    }
}
vector<string> getsplit2vec(string name,const string word){
    vector<string> temp;
    string::size_type pos1, pos2;
    pos2 = name.find(word);
    pos1 = 0;
    while(string::npos != pos2)
    {
        temp.emplace_back(name.substr(pos1, pos2-pos1));
        pos1 = pos2 + word.size();
        pos2 = name.find(word, pos1);
    }
    if(pos1 != name.length())
        temp.push_back(name.substr(pos1));
    return temp;
}
string unionwherelist(vector<Predicate> whereclause ,int site){
    int listsize = whereclause.size();
    if( listsize == 0)return "";
    else if(listsize == 1){
        pair<string,string> temp_pair = getsplit(whereclause[0].left_name, '.');
        temp_pair.first +="_"+to_string(site);
        whereclause[0].left_name = temp_pair.first + "." +temp_pair.second;
        return " WHERE " +  whereclause[0].to_string();
    }
    else{
        string where_stat = " WHERE ";
        for(int i = 0; i< listsize;i++){
            pair<string,string> temp_pair = getsplit(whereclause[i].left_name, '.');
            temp_pair.first +="_"+to_string(site);
            whereclause[i].left_name = temp_pair.first + "." +temp_pair.second;
            where_stat += whereclause[i].to_string();
            if(i<listsize-1)where_stat += " AND ";
        }
        return where_stat;
    }
} 
string unionvector(vector<string> myvector,string link){
    string result=myvector[0];
    for(int i = 1;i<myvector.size();i++){
        result+=link+myvector[i];
    }
    return result;
}
std::string justforbeauty(std ::string right){
    if(right.find("UNION")!=right.npos){
        return "UNION";
    }
    else if(right.find("JOIN")!=right.npos){
        return "JOIN";
    }
    else if(right.find("WHERE")!=right.npos){
        return "SELECTION";
    }
    else return "PROJECTION";
} 
void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}
string GetBefore(string sql_statement, string end) {
    if (sql_statement.find(end) == -1) {
        return "";
    }
    else {
        int end_loc = sql_statement.find(end);
        return sql_statement.substr(0,end_loc);
    }    
}
vector<string> GetList(string Line, string split, string stop) {
    vector <string> TableList;
    int loc_start = 0;
    int loc_end;
    int size = Line.size();
    int cnt = 0;
    while (size > 0) {
        loc_end = Line.find(split);
        int loc_table_end = loc_end + split.size();
        if (loc_end == -1) {
            string item = Line;
            item = item.substr(0,item.find(stop)-stop.size()+1);
            TableList.push_back(item);
            break;
        }
        else {
            string item = Line.substr(0,loc_end);
            TableList.push_back(item);
            size = size - loc_table_end;
            Line = Line.substr(loc_table_end,size);
        }       
    }
    return TableList;
}
string GetBetween(string sql_statement, string start, string end) {
    if (sql_statement.find(start) == -1 || sql_statement.find(end) == -1) {
        return "";
    }
    else {
        int start_loc = sql_statement.find(start) + start.size() + 1;
        int end_loc = sql_statement.find(end);
        return sql_statement.substr(start_loc, end_loc-start_loc);
    }
}
void Traversefrags(vector<FragDef> frags) {
    for (int i = 0; i < frags.size(); i++) {
        cout << "frags[" << to_string(i) << "] >>>" << endl;
        for(auto &col:frags[i].column){
            cout <<col<< "/" << endl;
        }
        for(auto &con:frags[i].condition){
            cout <<con<< "/" << endl;
        }
        cout << frags[i].id << "/" << endl;
        cout << frags[i].site << "/" <<endl;
        cout << frags[i].size << "/" << endl;
    }
}
string type2string(hsql::ColumnType coltype){
    switch(coltype.data_type){
        case hsql::DataType::CHAR:
            return "CHAR" + string("("+to_string(coltype.length)+")");
            break;
        case hsql::DataType::INT:
            return "INT";
            break;
        case hsql::DataType::DOUBLE:
            return "DOUBLE";
            break;
        case hsql::DataType::VARCHAR:
            return "VARCHAR" + string("("+to_string(coltype.length)+")");
            break;
        default:
            return "UNKNOWN";
    }

}
void DrawTree(vector<QTNode> Nodes) {
    ofstream out("draw.dot");
    int count = 1;
    std::string color;
    if (out.is_open()) {
        out << "digraph G {" << endl;
        for (int i = 0; i <Nodes.size(); i++) { 
            string item = to_string(Nodes[i].id) + "[label=" + to_string(Nodes[i].id);
            item += ", shape=box";
            item = item + ",label=\"" + Nodes[i].sql_statement + "\"";
            int site = Nodes[i].site;
            switch (site) {
                case 1:
                    color = ",color=red];";
                    break;
                case 2:
                    color = ",color=black];";
                    break;
                case 3:
                    color = ",color=green];";
                    break;
                case 4:
                    color = ",color=blue];";
                    break;
            }
            out << item + color << endl;
            if(Nodes[i].children.size() == 0){
                std::vector<std::string> meta_string;
                splitString(Nodes[i].sql_statement, meta_string," ");
                if(meta_string[3][meta_string[3].length()-1] == ';')meta_string[3][meta_string[3].length()-1] = ' ';
                else meta_string[3] = meta_string[3] + " ";
                out<<to_string(Nodes.size() + count ) + "[label=" + to_string(Nodes.size() + count ) 
                +", shape=box" + ",label=\""+ meta_string[3] + "site:" +to_string(Nodes[i].site) + "\"" + color<<endl;
                count++;
                string tmp = to_string(Nodes[i].id) +"->" + to_string(Nodes.size() + count - 1) + ";";
                out << tmp << endl;
            }
            for (int j = 0; j < Nodes[i].children.size(); j++) {
                string tmp = to_string(Nodes[i].id) +"->" + to_string(Nodes[i].children[j]) + ";";
                out << tmp << endl;
            }
        }
        out << "}" << endl;
        out.close();
    }
    else {
        cout << "ERROR : draw.dot is not open" << endl;
    }
}
void DrawTree2(vector<QTNode> Nodes) {
    ofstream out("drawtree.dot");
    if (out.is_open()) {
        out << "digraph G {" << endl;
        for (int i = 0; i < Nodes.size(); i++) { // id site sql_statement child
            string item = to_string(Nodes[i].id) + "[label=" + to_string(Nodes[i].id);
            item = item + ",label=\"" + Nodes[i].sql_statement + "\"";
            int site = Nodes[i].site;
            switch (site) {
                case 1:
                    item = item + ",color=red];";
                    break;
                case 2:
                    item = item + ",color=yellow];";
                    break;
                case 3:
                    item = item + ",color=green];";
                    break;
                case 4:
                    item = item + ",color=blue];";
                    break;
            }
            out << item << endl;
            for (int j = 0; j < Nodes[i].children.size(); j++) {
                string tmp = to_string(Nodes[i].children[j])+ "->"+to_string(Nodes[i].id)+";";
                out << tmp << endl;
            }
        }
        out << "}" << endl;
        out.close();
    }
    else {
        cout << "ERROR : drawtree.gv is not open" << endl;
    }
}
bool iscontain(string op1, string value1, string op2, string value2){
    //1是否包含2
    double num1 = stod(value1);
    double num2 = stod(value2);
    if(op1 == ">="){
        if(op2 == ">=" || op2 == ">" ){
            return true;
        }
        else if(op2 == "<=" || op2 =="="){
            if(num1 > num2)return false;
            else return true;
        }
        else if(op2 == "<"){
            if(num1 < num2)return true;
            else return false;
        }
    }
    else if(op1 == "<="){
        if(op2 == "<=" || op2 == "<" ){
            return true;
        }
        else if(op2 == ">=" || op2 =="="){
            if(num1 < num2)return false;
            else return true;
        }
        else if(op2 == ">"){
            if(num1 > num2)return true;
            else return false;
        }
    }
    else if(op1 == ">"){
        if(op2 == ">=" || op2 == ">" ){
            return true;
        }
        else if(op2 == "<=" || op2 =="="){
            if(num1 >= num2)return false;
            else return true;
        }
        else if(op2 == "<"){
            if(num1 < num2)return true;
            else return false;
        }
    }
    else if(op1 == "<"){
        if(op2 == "<=" || op2 == "<" ){
            return true;
        }
        else if(op2 == ">=" || op2 =="="){
            if(num1 <= num2)return false;
            else return true;
        }
        else if(op2 == ">"){
            if(num1 > num2)return true;
            else return false;
        }
    }
    else{
        cout<<"ERROR"<<endl;
        return false;
    }
}