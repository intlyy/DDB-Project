# ifndef UTILS_H
# define UTILS_H
# include <string>
# include <vector>
# include <fstream>
# include "Tree.h"
# include "Predicate.h"
# include "metadata.h"
# include "/home/liuwei/sql-parser-master/src/sql/ColumnType.h"
using namespace std;
pair<string,string> getsplit(string name, const char word);
vector<string> getsplit2vec(string name,const string word);
bool iscontain(string op1, string num1, string op2, string num2);
string unionwherelist(vector<Predicate> whereclause,int site);
string unionvector(vector<string> myvector,string link);
std::string justforbeauty(std ::string right);
void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
void DrawTree(vector<QTNode> Nodes);
void DrawTree2(vector<QTNode> Nodes);
string GetBetween(string sql_statement, string start, string end);
string GetBefore(string sql_statement, string end);
vector<string> GetList(string Line, string split, string stop);
// void Traversefrags(vector<FragDef> frags);
string type2string(hsql::ColumnType);
#endif