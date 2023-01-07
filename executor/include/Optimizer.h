#ifndef OPTIMIZER_H
#define OPTIMIZER_H
# define SITE_NUM 4
# include <string>
# include <map>
# include <iomanip>
# include <vector>
# include <iostream>

# include "Tree.h"
# include "metadata.h"
# include "SQLProcessor.h"
using namespace std;
class OptimizeMatrix{
    public:
    vector<QueryTable> query_tables;
    map<string,int> clause2idx;
    map<int,string> idx2clause;
    int N;   //列数
    int M = SITE_NUM + 2;  //行数
    int* matrix[SITE_NUM + 2];
    OptimizeMatrix(vector<QueryTable> query_tables);
    void InitMatrix(SQLProcessor * sqlp, map<string,Fragment> *FragmentMap);
    void printMatrix();
    ~OptimizeMatrix();
    auto Optimize(map<string,Fragment> FragmentMap);
    void SetNoFilter(string attribute, map<string,Fragment> *FragmentMap);
    void SetFilter(Fragment *fragment_tmp, Predicate *whereclause);
    QTree MatrixToTree(SQLProcessor *sqlp, map<string,Fragment> *FragmentMap);
};
#endif