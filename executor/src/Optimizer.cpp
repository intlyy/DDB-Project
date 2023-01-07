# include "../include/Optimizer.h"
# include "utils.h"
OptimizeMatrix::OptimizeMatrix(vector<QueryTable> tquery_tables):query_tables(move(tquery_tables)){
    int N_count = 0;
    for(auto temp_table : query_tables){
        for(auto temp_column : temp_table.column_names){
            clause2idx.emplace(pair<string,int> (temp_table.name + '.' + temp_column, 0));
            N_count++;
        }
    }
    int i = 0;
    for(auto &temp:clause2idx){
        temp.second = i; 
        idx2clause[i]=temp.first;
        i++;
    }
    N = N_count;
    for(int i = 0;i<M;i++){
        matrix[i] = new int[N]();  //申请内存并初始化为0
    }
    for(int i = 0; i < M; i ++){
        for(int j = 0; j<N; j++){
            matrix[i][j] = -1;
        }
    }
}
OptimizeMatrix::~OptimizeMatrix(){
    for(int i = 0; i < M; i++){
        delete[] matrix[i];
    }
}
void OptimizeMatrix::printMatrix(){
    for(auto &tmp:clause2idx){
        cout<<left<<setw(25)<<setfill(' ')<<tmp.first;
    }
    cout<<endl;
    for(int i = 0; i < M; i ++){
        for(int j = 0; j<N; j++){
            cout<<left<<setw(25)<<matrix[i][j];
        }
        cout<<endl;
    }
}
void OptimizeMatrix::SetNoFilter(string attribute, map<string,Fragment> *FragmentMap){
    pair<string,string> temp_pair = getsplit(attribute,'.');
    Fragment fragment_tmp = (*FragmentMap)[temp_pair.first];
    if(fragment_tmp.fragtype == "V"){
        for(auto &frag_tmp:fragment_tmp.frags){
            for(auto &temp:frag_tmp.column){
                if(temp == attribute){
                    matrix[frag_tmp.site - 1][clause2idx[attribute]] = 1;
                }
            }
        }
    }
    else{
        for(auto &frag_tmp:fragment_tmp.frags){
            matrix[frag_tmp.site - 1][clause2idx[attribute]] = 1;
        }
    }
}
void OptimizeMatrix::SetFilter(Fragment *fragment_tmp, Predicate *whereclause){
    int start = whereclause->left_name.length();
    for(auto &frag_tmp:(*fragment_tmp).frags){
        for(int i = 0;i < frag_tmp.column.size();i++){
            if(frag_tmp.column[i] == whereclause->left_name){
                string op_2 = frag_tmp.condition[i].substr(start, 2);
                if(op_2 == ">=" || op_2 == "<="){
                    if(iscontain(op_2, frag_tmp.condition[i].substr(start + 2),whereclause->op_type, whereclause->right_name)){
                        matrix[frag_tmp.site - 1][clause2idx[whereclause->left_name]] = 1;
                    }else{
                        matrix[frag_tmp.site - 1][clause2idx[whereclause->left_name]] = -1;
                    }
                }
                else{
                    string op_1 = frag_tmp.condition[i].substr(start, 1);
                    if(op_1 == "="){
                        if(whereclause->op_type.find("=") != whereclause->op_type.npos && frag_tmp.condition[i].substr(start +1) == whereclause->right_name){
                            matrix[frag_tmp.site - 1][clause2idx[whereclause->left_name]] = 1;
                        }else{
                            matrix[frag_tmp.site - 1][clause2idx[whereclause->left_name]] = -1;
                        }
                    }
                    else{
                        if(iscontain(op_1, frag_tmp.condition[i].substr(start + 1),whereclause->op_type, whereclause->right_name)){
                            matrix[frag_tmp.site - 1][clause2idx[whereclause->left_name]] = 1;
                        }
                        else{
                            matrix[frag_tmp.site - 1][clause2idx[whereclause->left_name]] = -1;
                        }
                    }
                }
            }
        }
    }
}
void OptimizeMatrix::InitMatrix(SQLProcessor *sqlp, map<string,Fragment> *FragmentMap){
    for(auto select: sqlp->select.select){
        SetNoFilter(select,FragmentMap);
        matrix[SITE_NUM][clause2idx[select]] = 1;
    }
    for(auto &from_table:sqlp->select.from){
        if((*FragmentMap)[from_table].fragtype == "V"){
            for(auto &frag_tmp:(*FragmentMap)[from_table].frags){
                matrix[frag_tmp.site-1][clause2idx[from_table+"."+getTableKey(from_table)]] = 1;
            }
            matrix[SITE_NUM+1][clause2idx[from_table+"."+getTableKey(from_table)]] = 1;
        }
    }
    for(auto &whereclause:sqlp->select.where){
        if(whereclause.pre_type == PredicateType::join){
            int lidx = clause2idx[whereclause.left_name];
            int ridx = clause2idx[whereclause.right_name];
            matrix[SITE_NUM+1][lidx] = ridx;
            matrix[SITE_NUM+1][ridx] = lidx;
            SetNoFilter(whereclause.left_name, FragmentMap);
            SetNoFilter(whereclause.right_name, FragmentMap);
        }
        else{
            //SET FILTER
            pair<string,string> temp_pair = getsplit(whereclause.left_name,'.');
            Fragment fragment_tmp = (*FragmentMap)[temp_pair.first];
            auto iter = fragment_tmp.fragcolumns.find(whereclause.left_name);
            if(matrix[SITE_NUM][clause2idx[whereclause.left_name]] != -1 || matrix[SITE_NUM+1][clause2idx[whereclause.left_name]] != -1){
                if(iter != fragment_tmp.fragcolumns.end() && fragment_tmp.fragtype == "H"){
                    SetFilter(&fragment_tmp, &whereclause);
                }
                if(matrix[SITE_NUM+1][clause2idx[whereclause.left_name]] != -1){
                    string attribute = idx2clause[matrix[SITE_NUM+1][clause2idx[whereclause.left_name]]];
                    pair<string,string> ttemp_pair = getsplit(attribute,'.');
                    Fragment fragment_ttmp = (*FragmentMap)[ttemp_pair.first];
                    auto tmpiter = fragment_ttmp.fragcolumns.find(attribute);
                    if(tmpiter != fragment_ttmp.fragcolumns.end() && fragment_ttmp.fragtype == "H"){
                        Predicate tmp_predicate(PredicateType::filter,attribute,whereclause.op_type,whereclause.right_name); 
                        SetFilter(&fragment_ttmp, &tmp_predicate);
                    }
                }
            }
            else{
                if(iter != fragment_tmp.fragcolumns.end()){
                    if(fragment_tmp.fragtype == "H"){
                        SetFilter(&fragment_tmp, &whereclause);
                    }
                    else{
                        for(auto &frag_tmp:fragment_tmp.frags){
                            for(auto &temp:frag_tmp.column){
                                if(temp == whereclause.left_name){
                                    matrix[frag_tmp.site - 1][clause2idx[whereclause.left_name]] = 1;
                                }
                            }
                        }
                    }
                }
                else{
                    for(auto &frag_tmp:fragment_tmp.frags){
                        matrix[frag_tmp.site - 1][clause2idx[whereclause.left_name]] = 1;
                    }
                }
            }
        }
    }
    bool flag;
    // 矩阵求和
    for(auto &rel:query_tables){
        Fragment fragment_tmp = (*FragmentMap)[rel.name];
        if(fragment_tmp.fragtype == "H"){
            for(int i = 0;i<SITE_NUM;i++){
                flag = true;
                for(auto &col:rel.column_names){
                    if(matrix[i][clause2idx[rel.name+"."+col]] == -1)flag = false;
                }
                if(!flag){
                    for(auto &col:rel.column_names){
                        matrix[i][clause2idx[rel.name+"."+col]] = -1;
                    }
                }
            }
        }
        else{
            for(int i = 0;i<SITE_NUM;i++){
                flag = false;
                for(auto &col:rel.column_names){
                    if(col != getTableKey(rel.name) && matrix[i][clause2idx[rel.name+"."+col]] == 1)flag = true;
                }
                if(!flag){
                    for(auto &col:rel.column_names){
                        matrix[i][clause2idx[rel.name+"."+col]] = -1;
                    }
                }
            }
        }
    }
}
QTree OptimizeMatrix::MatrixToTree(SQLProcessor *sqlp, map<string,Fragment> *FragmentMap){
    QTree tree;
    QTNode node;
    vector<QTNode> nodes;
    int treeid=0;
    int id_count = 0;
    map<string,pair<int,int>> join_idsite;
    for(auto &rel:query_tables){
        int node_count = 0;
        vector<Predicate> filter;
        for(auto &whereclause:sqlp->select.where){
            if(whereclause.pre_type == PredicateType::join)continue;
            pair<string,string> temp_pair = getsplit(whereclause.left_name,'.');
            if(temp_pair.first == rel.name){
                filter.emplace_back(whereclause);
            }
        }
        for(int i = 0; i < SITE_NUM; i++){
            string sql_state = "";
            for(auto &col:rel.column_names){
                int idx = clause2idx[rel.name+"."+col];
                if(matrix[i][idx] == 1 && (matrix[SITE_NUM][idx] != -1 || matrix[SITE_NUM + 1][idx] != -1)){
                    sql_state += (rel.name +"_" +to_string(i+1)+ "." + col + ",");
                }
            }
            if(sql_state == "") continue;
            sql_state.erase(sql_state.end() - 1);
            sql_state = "SELECT " + sql_state;
            sql_state += (" FROM " + rel.name + "_" + to_string(i+1));
            node.id = id_count;
            node.site = i+1;
            node_count++;
            if(!filter.empty()){
                if((*FragmentMap)[rel.name].fragtype == "V"){
                    vector<Predicate> V_filter;
                    for(auto &frag_tmp:(*FragmentMap)[rel.name].frags){
                        if(frag_tmp.site == i+1){
                            for(auto &temp:filter){
                                for(auto &col:frag_tmp.column){
                                    if(temp.left_name == col)V_filter.emplace_back(temp);
                                }
                            }
                        }
                    }
                    sql_state += unionwherelist(V_filter,node.site);
                }
                else{
                    sql_state += unionwherelist(filter,node.site);
                }
            }
            sql_state += ";";
            node.sql_statement = sql_state;
            nodes.emplace_back(node);
            id_count++;
        }
        if(node_count == 1){
            join_idsite.emplace(rel.name, make_pair(nodes[nodes.size()-1].id, nodes[nodes.size()-1].site));
            continue;
        }
        if((*FragmentMap)[rel.name].fragtype == "V"){
            // 按照主键连接
            node.id = id_count;
            node.site = nodes[nodes.size()-node_count].site;
            join_idsite.emplace(rel.name, make_pair(node.id, node.site));
            node.sql_statement = "SELECT * FROM ";
            for(int i = 0;i<node_count;i++){
                nodes[nodes.size()-i-1].parent = node.id;
                node.children.push_back(nodes[nodes.size()- i - 1].id);
                node.sql_statement += "tree_" + to_string(treeid) + "_node_" + to_string(nodes[nodes.size()-i-1].id);
                if(i<node_count-1)node.sql_statement += " JOIN ";
            }
            node.sql_statement += (" USING ("+getTableKey(rel.name)+");");
            nodes.emplace_back(node);
            node.children.clear();
            id_count++;
        }
        else{
            node.id = id_count;
            node.site = nodes[nodes.size()-node_count].site;
            node.sql_statement = "";
            join_idsite.emplace(rel.name, make_pair(node.id, node.site));
            for(int i = 0;i<node_count;i++){
                nodes[nodes.size()-i-1].parent = node.id;
                node.children.push_back(nodes[nodes.size() - i - 1].id);
                node.sql_statement += "SELECT * FROM tree_" + to_string(treeid) + "_node_" + to_string(nodes[nodes.size() - i - 1].id);
                if(i<node_count-1)node.sql_statement += " UNION ALL ";
            }
            node.sql_statement += ";";
            id_count++;
            nodes.emplace_back(node);
            node.children.clear();
        }
    }
    map<string,string> aliascol;
    map<string,int> showcol;
    for(auto tmp: clause2idx){
        if(matrix[SITE_NUM][tmp.second]!= -1 || matrix[SITE_NUM+1][tmp.second]!= -1)
            showcol[tmp.first] = 0;
            pair<string,string> name = getsplit(tmp.first,'.');
            //cout<<tmp.first<<endl;
            aliascol[tmp.first] = name.second;
    }
    for(auto &whereclause:sqlp->select.where){
        if(whereclause.pre_type == PredicateType::join){
            showcol[whereclause.left_name] +=1; 
            showcol[whereclause.right_name] +=1;
        }
    }
    for(auto &select:sqlp->select.select){
        showcol[select]+=1;
    }
    for(auto &whereclause:sqlp->select.where){
        if(whereclause.pre_type == PredicateType::join){
            pair<string,string> left_name = getsplit(whereclause.left_name,'.');
            pair<string,string> right_name = getsplit(whereclause.right_name,'.');
            pair<const string,pair<int,int>> *left_pair;
            pair<const string,pair<int,int>> *right_pair;
            int left_count;
            int right_count;
            vector<string> leftshow;
            vector<string> rightshow;
            for(auto &idsitemap:join_idsite){
                vector<string> names = getsplit2vec(idsitemap.first,",");
                bool lflag = false;
                bool rflag = false;
                for(auto &name:names){
                    if(name == left_name.first){
                        showcol[whereclause.left_name]-=1;
                        left_pair = &idsitemap;
                        left_count = names.size();
                        lflag = true;
                    }
                    if(name == right_name.first){
                        showcol[whereclause.right_name]-=1;
                        right_pair = &idsitemap;
                        right_count = names.size();
                        rflag = true;
                    }
                }
                if(lflag){
                    for(auto &tmp:showcol){
                        if(tmp.second > 0){
                            pair<string,string> left_name = getsplit(tmp.first,'.');
                            for(auto &name:names){
                                if(name == left_name.first){
                                    leftshow.emplace_back(tmp.first);
                                }
                            }
                        }
                    }
                }
                if(rflag){
                    for(auto &tmp:showcol){
                        if(tmp.second > 0){
                            pair<string,string> right_name = getsplit(tmp.first,'.');
                            for(auto &name:names){
                                if(name == right_name.first){
                                    rightshow.emplace_back(tmp.first);
                                }
                            }
                        }
                    }
                }
            }
            
            if(left_pair == right_pair)continue;// 可以做一次投影
            else{
                node.id = id_count;
                id_count++;
                if(left_count>=right_count)node.site = (*left_pair).second.second;
                else node.site = (*right_pair).second.second;
                node.children.push_back(left_pair->second.first);
                node.children.push_back(right_pair->second.first);
                join_idsite.emplace(left_pair->first+","+right_pair->first,make_pair(node.id,node.site));
                join_idsite.erase(left_pair->first);
                join_idsite.erase(right_pair->first);
                for(auto &tmpnode:nodes){
                    if(tmpnode.id == node.children[0] || tmpnode.id == node.children[1]){
                        tmpnode.parent = node.id;
                    }
                }
                string new_left = "tree_" + to_string(treeid) + "_node_" + to_string(node.children[0]);
                string new_right = "tree_" + to_string(treeid) + "_node_" + to_string(node.children[1]);
                node.sql_statement = "SELECT ";
                string las = "";
                string ras = "";
                string lcol = "";
                string rcol = "";
                for(auto leftcol:leftshow){
                    pair<string,string> left_name = getsplit(leftcol,'.');
                    for(auto rightcol:rightshow){
                        pair<string,string> right_name = getsplit(rightcol,'.');
                        if(left_name.second == right_name.second){
                            lcol = leftcol;
                            rcol = rightcol;
                            las = left_name.first + "_"+left_name.second;
                            ras = right_name.first+"_"+right_name.second;
                            
                        }
                    }
                }
                for(auto tmpcol:leftshow){
                    if(lcol ==tmpcol){
                        node.sql_statement += new_left + "." +aliascol[tmpcol] +" AS " +las+",";
                        aliascol[lcol] = las;
                    }
                    else node.sql_statement += new_left + "." +aliascol[tmpcol] +",";
                }
                for(auto tmpcol:rightshow){
                    if(rcol == tmpcol){
                        node.sql_statement += new_right + "." +aliascol[tmpcol]  +" AS " +ras+",";
                        aliascol[rcol] = ras;
                    }
                    else node.sql_statement += new_right + "." +aliascol[tmpcol] +",";
                }
                node.sql_statement.erase(node.sql_statement.end()-1);
                node.sql_statement += " FROM " + new_left + "," + new_right + " WHERE " + new_left + "." 
                + left_name.second + "=" + new_right + "." + right_name.second + ";";
                nodes.emplace_back(node);
                node.children.clear();
            }
        }
    }
    int count = 1;
    for(auto &tmp:join_idsite){
        node.id = id_count;
        node.children.push_back(tmp.second.first);
        nodes[nodes.size()-count].parent =  node.id;
        node.parent = -1 - count;
        id_count+=2;
        count+=2;
        node.site = tmp.second.second;
        node.sql_statement = "SELECT ";
        for(auto &select:sqlp->select.select){
            cout<<select<<endl;
            pair<string,string> temp_pair = getsplit(select,'.');
            vector<string> names = getsplit2vec(tmp.first,",");
            for(auto &name:names){
                if(temp_pair.first == name){
                    node.sql_statement += aliascol[select] + ",";
                }
            }
        }
        node.sql_statement.erase(node.sql_statement.end() - 1);
        node.sql_statement += (" FROM " + string("tree_") + to_string(treeid) + "_node_" + to_string(node.children[0]) + ";");
        nodes.emplace_back(node);
        node.children.clear();
    }
    for(auto &node:nodes){
        cout<<node.site<<'\t'<<node.sql_statement<<endl;
    }
    tree.Nodes = nodes;
    tree.tree_id = treeid;
    tree.root = nodes[nodes.size()-1].id;
    return tree;
}
auto OptimizeMatrix::Optimize(map<string,Fragment> FragmentMap){
    //TODO  optimize join order and select site
}

