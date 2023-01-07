#include<string>
using namespace std;
# include "../include/Tree.h"
# include "../include/SQLProcessor.h"
# include "../include/Optimizer.h"
# include "../include/metadata.h"
# include "../include/utils.h"
# include "../include/executor.h"

void TraverseInt(vector<int> intlist) {
    for (int i = 0; i < intlist.size(); i++){
        cout << intlist[i] << endl;
    }
}
void TraverseTree(vector<QTNode> tree) {
    cout<<"tree.size:"<<tree.size()<<endl;
    for(int i = 0; i < tree.size(); i++) {
        cout << "=="<< tree[i].id << "==" << endl;
        cout << "site =>" << tree[i].site << endl;
        cout << "sql_statement =>" << tree[i].sql_statement<< "/" << endl;
        cout << "child =>" << endl;
        TraverseInt(tree[i].children);
        cout << "parent =>" << endl;
        cout << tree[i].parent << endl;
    }
}
int main(){
    std::string query_test2 = "delete from orders";
    std::string query3 = "select customer.name from customer where rank<3 and id>0";
    std::string query_test = "insert into customer(id, name, rank) values(1,'High Education Press', 2);";
    std::string query = "SELECT custosmer.name,book.title,publisher.name,orders.quantity FROM customer,book,publisher,orders WHERE customer.id=orders.customer_id and book.id=orders.book_id and book.publisher_id=publisher.id and customer.id>308000 and book.copies>100 and orders.quantity>1 and publisher.nation='PRC';";
    std::string query2 ="SELECT customer.name,orders.quantity,book.title FROM customer,orders,book WHERE customer.id=orders.cusomer_id and book.id=orders.book_id and customer.rank=1 and book.copies>5000;";
    std::string query4 = "CREATE TABLE book(id INT,title CHAR(100),authors CHAR(100),publisher_id INT,copies INT, PRIMARY KEY (id));";
    std::string query5 = "CREATE FRAGMENTATION customer( V,1 customer.id customer.name 1,2 customer.id customer.rank 2);";
    //3 orders.customer_id>=307000 orders.book_id<215000 3,4 orders.cusomer_id>=307000 orders.book_id>=215000 4
    //CREATE FRAGMENTATION book( H,1 book.id<205000 1,2 book.id>=205000 book.id<210000 2,3 book.id>=210000 3);
    // 
    std::string query6 = "COPY orders FROM '/home/liuwei/DDB-Project/data/orders.tsv';";
    string str;
    cout<<"欢迎使用NaN小组分布式数据库"<<endl;
    while(1)
    {
        cout<<endl;
        cout<<"请输入sql语句"<<endl;
        
        getline(cin, str);
        
        //cout << str << endl;
        if(str.compare("exit")==0) return 0;
        if(str.find("show")!=-1)
        {
            vector<string> tmp=getsplit2vec(str," ");
            if(str.find("table")!=-1)
            {
                getTableFromEtcd(tmp[2]);
            }
            else if(str.find("fragment")!=-1)
            {
                getFragFromEtcd(tmp[2]);
            }
            else
            {
                cout<<"语句错误！"<<endl;
            }
            continue;
            
        }

        SQLProcessor sqlp = SQLProcessor(str);
        //map<string,RelationDef> relation_map = Getrelationmap();
        //map<string,string> TableKeyMap = GetETCDTableKey();
        // sql_type 1:select 2:insert 3.delete 
        
        if(sqlp.is_valid){
            if(sqlp.sql_type == 1){
                vector<QueryTable> query_tables;
                map<string, Fragment> FragmentMap = GetETCDFragment();
                time_t start_time = time(NULL);
                for(auto from_table: sqlp.select.from){
                    QueryTable temp_table;
                    temp_table.name = from_table;
                    if(FragmentMap[from_table].fragtype == "V")temp_table.column_names.emplace(getTableKey(from_table));
                    for(auto select: sqlp.select.select){
                        pair<string,string> temp_pair = getsplit(select,'.');
                        if(temp_pair.first == temp_table.name){
                        temp_table.column_names.emplace(temp_pair.second);
                        }
                    }
                    for(auto where: sqlp.select.where){
                        if(where.pre_type == PredicateType::filter){
                            pair<string,string> temp_pair = getsplit(where.left_name,'.');
                            if(temp_pair.first == temp_table.name){
                                temp_table.column_names.emplace(temp_pair.second);
                            }
                        }
                        if(where.pre_type == PredicateType::join){
                            pair<string,string> temp_pair1 = getsplit(where.left_name,'.');
                            pair<string,string> temp_pair2 = getsplit(where.right_name,'.');
                            if(temp_pair1.first == temp_table.name){
                                temp_table.column_names.emplace(temp_pair1.second);
                            }
                            else if(temp_pair2.first == temp_table.name){
                                temp_table.column_names.emplace(temp_pair2.second);
                            }
                        }
                    }
                    query_tables.emplace_back(temp_table);
                }
                OptimizeMatrix opm = OptimizeMatrix(query_tables);
                opm.InitMatrix(&sqlp, &FragmentMap);
                opm.printMatrix();
                QTree tree = opm.MatrixToTree(&sqlp, &FragmentMap);
                DrawTree(tree.Nodes);
                TraverseTree(tree.Nodes);

                ETree exe=Data_Select_Execute(tree);
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
                time_t end_time = time(NULL);
                double time_spend = difftime(end_time, start_time);
                string p="dot -Tpng draw.dot -o draw.png";
                int tmp=system(p.data());
                cout<<"查询完成！共用时： "<<time_spend<<"s"<<endl;
                res_print("tree_"+to_string(exe.tree_id)+"_node_"+to_string(exe.root));

            }
            else{
                vector<string> othersql;
                vector<int> sites;
                if(sqlp.sql_type == 2){
                    vector<string> insertvalues = getsplit2vec(sqlp.insert.values, ", ");
                    RelationDef insertrelation = getTableFromEtcd(sqlp.insert.rname);
                    Fragment fragment_tmp = getFragFromEtcd(sqlp.insert.rname);
                    
                    if(fragment_tmp.fragtype == "V"){
                        for(int i = 0;i<fragment_tmp.fragnum;i++){
                            string insert1 = "INSERT INTO " + sqlp.insert.rname + "_" + to_string(fragment_tmp.frags[i].site) +"(";
                            string insert2 = " VALUES(";
                            for(auto colname:fragment_tmp.frags[i].column){
                                pair<string,string> temp_pair = getsplit(colname,'.');
                                for(int j = 0;j<insertrelation.cols.size();j++){
                                    if(insertrelation.cols[j].name == temp_pair.second){
                                        insert1 += (temp_pair.second + ",");
                                        insert2 += (insertvalues[j] + ",");
                                    }
                                }
                            }
                            insert1.erase(insert1.end() - 1);
                            insert2.erase(insert2.end() - 1);
                            insert1 += ")";
                            insert2 += ")";
                            othersql.emplace_back(insert1+insert2);
                            sites.push_back(fragment_tmp.frags[i].site);
                        }
                    }
                    else{
                        map<string,string> fragcol2value;
                        for(auto &col:fragment_tmp.fragcolumns){
                            pair<string,string> temp_pair = getsplit(col,'.');
                            for(int i = 0;i<insertrelation.cols.size();i++){
                                if(temp_pair.second == insertrelation.cols[i].name){
                                    fragcol2value[col] = insertvalues[i];
                                }
                            }
                        }
                        bool flag = false;
                        for(auto &frag_tmp:fragment_tmp.frags){
                            for(int i = 0;i < frag_tmp.column.size();i++){
                                int start = frag_tmp.column[i].length();
                                string op = frag_tmp.condition[i].substr(start, 2);
                                if(op == ">=" || op == "<="){
                                    if(iscontain(op,frag_tmp.condition[i].substr(start + 2),"=",fragcol2value[frag_tmp.column[i]])){
                                        if(i<frag_tmp.column.size()-1)continue;
                                    }
                                    else break;
                                }
                                else{
                                    op = frag_tmp.condition[i].substr(start, 1);
                                    if(op == "="){
                                        if(frag_tmp.condition[i].substr(start +1) == fragcol2value[frag_tmp.column[i]]){
                                            if(i<frag_tmp.column.size()-1)continue;
                                        }
                                        else break;
                                    }
                                    else{
                                        if(iscontain(op,frag_tmp.condition[i].substr(start + 1),"=",fragcol2value[frag_tmp.column[i]])){
                                            if(i<frag_tmp.column.size()-1)continue;
                                        }
                                        else break;
                                    }
                                }
                                flag = true;
                            }
                            if(flag){
                                sites.push_back(frag_tmp.site);
                                string tempsql = "INSERT INTO " + sqlp.insert.rname + "_" + to_string(frag_tmp.site) + "(";
                                for(auto &col:insertrelation.cols){
                                    tempsql += (col.name + ",");
                                }
                                tempsql.erase(tempsql.end() - 1);
                                tempsql += (") VALUES(" + sqlp.insert.values + ");");
                                othersql.emplace_back(tempsql);
                                break;
                            }
                        }
                    }
                    cout<<Data_Insert_Delete_Execute(sites,othersql);
                }
                else if(sqlp.sql_type == 3){
                    Fragment fragment_tmp = getFragFromEtcd(sqlp.delete_s.rname);
                    string tmpdelete = "DELETE FROM " + sqlp.delete_s.rname;
                    for(auto &frag_tmp:fragment_tmp.frags){
                        othersql.emplace_back(tmpdelete + "_" + to_string(frag_tmp.site)+";");
                        sites.push_back(frag_tmp.site);
                    }
                    cout<<Data_Insert_Delete_Execute(sites,othersql);
                }
                else if(sqlp.sql_type == 4){
                    saveTableToEtcd(sqlp.create_table);
                    vector<string> exist_tables = getTables();
                    for(auto &name:exist_tables){
                        getTableFromEtcd(name);
                    }
                }
                else if(sqlp.sql_type == 5){
                    RelationDef import_relation = getTableFromEtcd(sqlp.import_s.tablename);
                    Fragment frag = getFragFromEtcd(sqlp.import_s.tablename);
                    string create_sql = "CREATE TABLE `" + sqlp.import_s.tablename + "_temp`(";
                    for(auto &relcol:import_relation.cols){
                        create_sql += ("`"+relcol.name + "` " + relcol.type + ",");
                    }
                    create_sql.erase(create_sql.end() - 1);
                    create_sql+=");";
                    string load_sql = "LOAD DATA LOCAL INFILE '"+sqlp.import_s.filepath + "' INTO TABLE `"+sqlp.import_s.tablename + "_temp`;";
                    if(frag.fragtype == "V"){
                        for(auto &onefrag:frag.frags){
                            string tmpsql_V = "SELECT ";
                            for(auto &cond:onefrag.condition){
                                pair<string,string> tmp_pair = getsplit(cond,'.');
                                tmpsql_V+=tmp_pair.second+",";
                            }
                            tmpsql_V.erase(tmpsql_V.end()-1);
                            tmpsql_V+=" FROM "+sqlp.import_s.tablename + "_temp;";
                            othersql.emplace_back(tmpsql_V);
                            sites.emplace_back(onefrag.site);
                        }
                    }
                    else if(frag.fragtype == "H"){
                        for(auto &onefrag:frag.frags){
                            string tmpsql_H = "SELECT * FROM "+ sqlp.import_s.tablename + "_temp WHERE ";
                            for(auto &cond:onefrag.condition){
                                pair<string,string> tmp_pair = getsplit(cond,'.');
                                tmpsql_H+=tmp_pair.second+" and ";
                            }
                            tmpsql_H = tmpsql_H.substr(0,tmpsql_H.length()-5);
                            tmpsql_H+=";";
                            othersql.emplace_back(tmpsql_H);
                            sites.emplace_back(onefrag.site);
                        }
                    
                    }
                    cout<<create_sql<<endl;
                    cout<<load_sql<<endl;
                    vector<string> table_names;
                    for(int i=0;i<sites.size();i++) table_names.push_back(sqlp.import_s.tablename+"_"+to_string(i+1));
                    string main_name= sqlp.import_s.tablename+"_temp";
                    cout<<Data_Load_Execute(create_sql, load_sql, main_name, sites, othersql,  table_names);
                }
                else if(sqlp.sql_type == 6){
                    if(!sqlp.createfrag_name.empty()){
                        vector<string> exist_tables = getTables();
                        bool isexist = false;
                        for(auto &exist:exist_tables){
                            if(exist == sqlp.createfrag_name){
                                isexist = true;
                                break;
                            }
                        }
                        if(isexist){
                            RelationDef create_relation = getTableFromEtcd(sqlp.createfrag_name);
                            Fragment frag = getFragFromEtcd(sqlp.createfrag_name);
                            //Fragment frag = GetETCDFragmentByName(sqlp.createfrag_name);
                            string rel_key = getTableKey(sqlp.createfrag_name);
                            if(frag.fragtype == "H"){
                                for(auto &onefrag:frag.frags){
                                    string tmpsql = "CREATE TABLE IF NOT EXISTS `" + sqlp.createfrag_name +"_"+to_string(onefrag.site)+ "`(";
                                    for(auto &relcol:create_relation.cols){
                                        tmpsql += ("`"+relcol.name + "` " + relcol.type + ",");
                                    }
                                    if(!rel_key.empty()){
                                        tmpsql += ("PRIMARY KEY(`"+rel_key+"`));");
                                    }
                                    else{
                                        tmpsql.erase(tmpsql.end() - 1);
                                        tmpsql+=");";
                                    }
                                    //cout<<tmpsql<<endl;
                                    othersql.emplace_back(tmpsql);
                                    sites.emplace_back(onefrag.site);
                                }
                            }
                            else if(frag.fragtype == "V"){
                                for(auto &onefrag:frag.frags){
                                    string tmpsql = "CREATE TABLE IF NOT EXISTS `" + sqlp.createfrag_name +"_"+ to_string(onefrag.site) + "`(";
                                    for(auto &col:onefrag.column){
                                        pair<string,string> tmp_pair = getsplit(col,'.');
                                        for(auto &relcol:create_relation.cols){
                                            if(relcol.name == tmp_pair.second){
                                                tmpsql += ("`"+tmp_pair.second + "` " + relcol.type + ",");
                                                break;
                                            }
                                        }
                                    }
                                    if(!rel_key.empty()){
                                        tmpsql += ("PRIMARY KEY(`"+rel_key+"`));");
                                    }
                                    else{
                                        tmpsql.erase(tmpsql.end() - 1);
                                        tmpsql+=";";
                                    }
                                    // cout<<tmpsql<<endl;
                                    othersql.emplace_back(tmpsql);
                                    sites.emplace_back(onefrag.site);
                                }
                            }
                            cout<<Data_Insert_Delete_Execute(sites,othersql);
                        }
                        else{
                            cout<<"Relation has not been built!"<<endl;
                        }
                    }
                    else{cout<<"NULLLLLLLLLLLLL"<<endl;}
                }
                for(auto &tmpsql:othersql){
                    cout<<tmpsql<<endl;
                }
                for(auto &tmp:sites) cout<<tmp<<endl;
            }
        }
        getchar();
        
    }
    return 0;
}




