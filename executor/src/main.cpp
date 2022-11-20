/*
condition_list in GetTCCListTest>>>
table_name in SELECT :customer/
join done
SELECT * FROM customer;/
==1==
site =>1
sql_statement =>SELECT * FROM customer_1/
child =>
parent =>
3
==2==
site =>2
sql_statement =>SELECT * FROM customer_2/
child =>
parent =>
3
==3==
site =>1
sql_statement =>SELECT * FROM tree_0_node_2 JOIN tree_0_node_1 USING (id)/
child =>
2
1
parent =>
0
*/
#include "../include/SELECT_sta.h"
#include "../include/executor.h"
#include<string>
using namespace std;

int main()
{
    string sql_statement;
    QTree Tree;
    sql_statement = "SELECT * FROM customer;";
    Tree = SELECT(sql_statement,0);
    cout << sql_statement << "/" << endl;
    TraverseTree(Tree.Nodes);
    DrawTree(Tree.Nodes);

    ETree exe=Data_Select_Execute(Tree);
    ETNode root = getroot(exe);
    for (int i = 0; i < exe.Nodes.size(); i++) {
                cout << "node_id :" << exe.Nodes[i].id << endl ;
                cout << "time_spend :" << exe.Nodes[i].time_spend << endl;
                cout << "volume :" << exe.Nodes[i].vol << endl;
                cout << "res :" << exe.Nodes[i].res << endl;
                cout << "child :" << endl;
                TraverseInt(exe.Nodes[i].children);
                cout << "parent :" << exe.Nodes[i].parent << endl;
                cout << "site :" << exe.Nodes[i].site << endl;
            }
    res_print("tree_"+to_string(exe.tree_id)+"_node_"+to_string(exe.root));
}
