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
    /*
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
    */
   string sql_statement;
    // in load function
    vector<string> sql_statements;
    string create_sql_yq;
    string load_sql_yq;
    string main_name;
    vector<string> sitenames;
    vector<string> sqls;
    vector<string> table_names;
    int treeid = 0;
    while (true) 
    {
        sql_statement = "";
        cout << ">>>";
        getline(cin,sql_statement);
        
        //sql_statement = "SELECT * FROM customer;";
        if(sql_statement.find("SELECT") != -1)
        {
            QTree Tree;
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
        else if(sql_statement.find("INSERT") != -1)
        {
           //INSERT INTO customer VALUES(3,shc,3);
            cout<<"Please input table and values:"<<endl;
            string ss,id,name,r;cin>>ss;
            cin>>id>>name>>r;
            vector<int> site;
            site.push_back(1);
            site.push_back(2);
            vector<string> sql;
            string s1="insert into customer_1 values("+id+","+r+")";
            string s2="insert into customer_2 values("+id+","+name+")";
            cout<<s1<<endl;
            cout<<s2<<endl;
            sql.push_back(s1);
            sql.push_back(s2);
            cout<<Data_Insert_Delete_Execute(site,sql)<<endl;
        }
        else if(sql_statement.find("DELETE") != -1)
        {
            cout<<"Please input table and key:"<<endl;
            string ss,id,name,r;cin>>ss;
            cin>>id;
            vector<int> site;
            site.push_back(1);
            site.push_back(2);
            vector<string> sql;
            string s1="delete from customer_1 where id="+id;
            string s2="delete from customer_2 where id="+id;
            cout<<s1<<endl;
            cout<<s2<<endl;
            sql.push_back(s1);
            sql.push_back(s2);
            cout<<Data_Insert_Delete_Execute(site,sql)<<endl;
        }
        else if(sql_statement.find("COPY") != -1)
        {
            string create_sql="create table customer_tmp (id int NOT NULL, name char(100) ,r int DEFAULT NULL, PRIMARY KEY (id))";
            string load_sql=" LOAD DATA LOCAL INFILE '/mnt/d/DDB/customer.tsv' INTO TABLE customer_tmp";
            string main_name="customer_tmp";
            vector<int> sitenames;
            sitenames.push_back(1);
            sitenames.push_back(2);
            vector<string> sqls;
            sqls.push_back("select id,name from customer_tmp");
            sqls.push_back("select id,r from customer_tmp");
            vector<string> table_names;
            table_names.push_back("customer_1");
            table_names.push_back("customer_2");
            cout<<Data_Load_Execute(create_sql, load_sql, main_name,  sitenames, sqls,  table_names);
            return 0;
        }
        else if(sql_statement.find("EXIT") != -1)
        {
            cout<<"Bye!"<<endl;
            return 0;
        }
    }
}
