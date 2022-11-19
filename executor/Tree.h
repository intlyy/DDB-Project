#ifndef mytree
#define mytree
#include<iostream>
#include<vector>
#include<string>
#include<cassert>
using namespace  std;


struct QTNode {
	int id;
	vector<int> children;
	int parent;
	string sql_statement;
	int site;
};
struct QTree {
    int tree_id;
    int root;
    vector<QTNode> Nodes;
};
struct ETNode{
    int id;
    size_t vol;//结果数据量大小（以byte为单位  ）
    string res;
    vector<int> children;
    int parent;
    int site;
    double time_spend;
};
struct ETree{
    int tree_id;
    int root;
    vector<ETNode> Nodes;
};

QTNode getroot(QTree tree);

ETNode getroot(ETree tree);

QTree getsubTree(QTree tree, int  id);

QTNode getnode(QTree tree,int id);

#endif

