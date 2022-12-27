#include "../include/executor.h"

void Data_Insert_Delete_Thread(int site, string frag_sql, std::promise<string> &resultObj){
    /* 判断一下是否为本地，并执行对应sql语句 */
    string frag_res;
    string res_output;
    if(site == LOCALSITE || site == LOCALSITE2){
        frag_res = Insert(frag_sql, site);
        printf("localsite.\n%s\n", frag_res.data());
    }
    else{
        printf("not localsite.");
        //此函数需要实现！在站点site上调用Insert(frag_sql, site)并返回结果
        frag_res = RPC_Insert(site, frag_sql);
        
        // printf("success .");
        // printf("%s", frag_res.data());
        // cout << frag_res;
    }
    // frag_res = "OK";
    if(frag_res == "OK"){ /* 目标site上存储成功 */
        /* 构造输出语句 */
        res_output = "OK on site ";
        string s=to_string(site);
        res_output.append(s);
        res_output.append(".\n");
        
        // printf("%s\n", res_output.data());
        // return res_output;
    }
    else{
        // printf("FAIL TO LOAD %s", frag_name);
        res_output = "FAIL on site ";
        string s=to_string(site);
        res_output.append(s);
        res_output.append(".\n");
        // printf("%s\n", res_output.data());
        // return res_output_2;
    }
    resultObj.set_value(res_output);
}

/* 本函数供parser调用
本函数用于执行整个Insert或Delete流程，输入站点列表，分片sql语句列表，返回
"OK/FAIL on site 1.
 OK/FAIL on site 2.
 y seconds used." */
string Data_Insert_Delete_Execute(vector<int> sitenames, vector<string> sqls)
{
    /* 记录开始时间 */
    time_t start_time = time(NULL);
    int i;
    /* 先把空间给申请好 */
    std::promise<string> resultObjs[MAXTHREAD]; 
    std::thread load_threads[MAXTHREAD];
    std::future<string> load_sentences[MAXTHREAD];

    for(i = 0; i < sitenames.size(); i++){           
        /* 开启一个分片并在对应site存储的线程，通过传promise类给线程，让线程把结果给future类，实现结果返回 */
        load_sentences[i] = resultObjs[i].get_future();
        load_threads[i] = std::thread(Data_Insert_Delete_Thread, sitenames[i], sqls[i], std::ref(resultObjs[i]));
    }

    /* 从每个进程中获得返回结果并汇总 */
    vector<string> load_results;
    for(i = 0; i < sitenames.size(); i++){
        string load_sentence = load_sentences[i].get();
        load_results.push_back(load_sentence);
    }

    /* 本意是说所有线程完成了主函数才继续 */
    for(i = 0; i < sitenames.size(); i++){
        load_threads[i].join();
    }
    /* 计算所花时间 */
    time_t end_time = time(NULL);
    double time_spend = difftime(end_time, start_time);
    /* 构造输出语句 */
    string time_output = to_string(time_spend);
    time_output.append(" seconds used.\n");
    string output_sentence = "";
    for(i = 0; i < load_results.size(); i++){
        output_sentence.append(load_results[i]);
    }
    output_sentence.append(time_output);
    return output_sentence;
}

ETree Data_Select_Execute(QTree tree)
{
    QTree sub_tree;
    ETree res_tree;
    QTNode child_node;
    res_tree.tree_id=tree.tree_id;
    res_tree.root=tree.root;
    
    time_t start_time = time(NULL);

    QTNode root=getroot(tree);
    vector<int> children = root.children;
    ETNode Exec_root;
    Exec_root.id = root.id;
    Exec_root.site = root.site;
    Exec_root.children = root.children;
    Exec_root.parent = root.parent;

    //叶节点
    if(tree.Nodes.size() == 1)
    {
        string res_name = "tree_" + to_string(tree.tree_id) + "_node_" + to_string(root.id);
        string select_res;
        if( root.site == LOCALSITE)
        {
            select_res=Select(root.sql_statement, root.site, res_name);
        }
        else
        {
	    std::cout << "root site is: " << root.site << ", but localsite is: " << LOCALSITE << std::endl;
            printf("site do not in this station!");
        }
	std::cout<<"select_res:"<<select_res<<std::endl;
        if(select_res=="FAIL")
        {
            time_t end_time = time(NULL);
            double time_spend = difftime(end_time, start_time);
            Exec_root.time_spend = time_spend;
            Exec_root.vol = 0;
            Exec_root.res = "FAIL";
            res_tree.Nodes.push_back(Exec_root);
            return res_tree;
        }
        else
        {
            string filepath = TMPPATH + res_name + ".sql";
            const char* p = filepath.data();
            //size_t res_volume = get_filebytes(p);
            size_t res_volume = 1;
            time_t end_time = time(NULL);
            double time_spend = difftime(end_time, start_time);
            Exec_root.time_spend = time_spend;
            Exec_root.vol = res_volume;
            Exec_root.res = "OK";
            res_tree.Nodes.push_back(Exec_root);
            return res_tree;
        }
        
    } 
    else
    {
        //非叶节点
        /* 遍历根节点的子节点，获得以该子节点为根节点的子树，这里的遍历先写成循环，通顺以后再改成多线程  */
        /* 先把空间给申请好 */ 
        std::promise<ETree> resultObjs[MAXTHREAD]; 
        std::thread select_threads[MAXTHREAD];
        std::future<ETree> select_thread_exec_trees[MAXTHREAD];
        for(int q=0; q<children.size();q++)
        {
            printf("child %d on processing ...\n", children[q]);
            sub_tree = getsubTree(tree,children[q]);
            child_node = getnode(tree, children[q]);
            int site = child_node.site;

            select_thread_exec_trees[q] = resultObjs[q].get_future();

            if(site == LOCALSITE)
            {
                select_threads[q] = std::thread(Data_Select_Execute_thread, sub_tree, std::ref(resultObjs[q]));
            }
            else
            {
                printf("remote call in site %d\n", site);
                select_threads[q] = std::thread(RPC_Data_Select_Execute_Thread, sub_tree, site, std::ref(resultObjs[q]));
            }
        }
        printf("create ok!");
        for(int q=0; q<children.size(); q++)
        {
            string res_name = "tree_" + to_string(tree.tree_id) + "_node_" + to_string(children[q]);
            ETree res_sub_tree = select_thread_exec_trees[q].get();
            ETNode res_sub_root = getroot(res_sub_tree);
            if(res_sub_root.res=="OK")
            {
                for(ETNode tmp_exec_node: res_sub_tree.Nodes)
                {
                    res_tree.Nodes.push_back(tmp_exec_node);
                }
                string tmp_load_res = tmp_load(res_name, root.site);
                if(tmp_load_res == "OK"){
                    printf("tmp table loading of %s succeed!\n", res_name.data());
                }
                else
                {
                    time_t end_time = time(NULL);
                    double time_spend = difftime(end_time, start_time);
                    Exec_root.time_spend = time_spend;
                    Exec_root.vol = 0;
                    Exec_root.res = "FAIL";
                    res_tree.Nodes.push_back(Exec_root);
                    return res_tree;
                }
            }
            else
            {
                time_t end_time = time(NULL);
                double time_spend = difftime(end_time, start_time);
                Exec_root.time_spend = time_spend;
                Exec_root.vol = 0;
                Exec_root.res = "FAIL";
                res_tree.Nodes.push_back(Exec_root);
                return res_tree;
            }
        }
        printf("Exe ok!");
        for(int q=0; q<children.size(); q++){
            select_threads[q].join();
        }
        printf("Exe2 ok!");
    }

    /* 根据根节点的sql_statement执行结果整合 */
    string root_sql = root.sql_statement;
    string root_res_name = "tree_" + to_string(tree.tree_id) + "_node_" + to_string(root.id);
    int root_site = root.site;
    string root_select_res = Select(root_sql,  root_site, root_res_name);

    /*
        清理数据库
        算了不想写了
        算了还是得写
     */
    for(int child_id: children)
    {
         string drop_sql = "drop table tree_" + to_string(tree.tree_id) + "_node_" + to_string(child_id);
         string drop_res = Mysql_Delete(drop_sql, root.site);
         if(drop_res != "OK"){
             std::cout<<"release failed! "<< drop_sql.data()<<std::endl;
         }
    }

    if(root_select_res == "FAIL")
    {
        time_t end_time = time(NULL);
        double time_spend = difftime(end_time, start_time);
        Exec_root.time_spend = time_spend;
        Exec_root.vol = 0;
        Exec_root.res = "FAIL";
        res_tree.Nodes.push_back(Exec_root);
        return res_tree;
    }
    else
    {
        string root_filepath = TMPPATH + root_res_name + ".sql";
        const char* root_p = root_filepath.data();
        //size_t root_res_volume = get_filebytes(root_p);
        size_t root_res_volume = 1;

        time_t end_time = time(NULL);
        double time_spend = difftime(end_time, start_time);
        /* 构造本节点的执行记录，合并进执行树 */
        Exec_root.time_spend = time_spend;
        Exec_root.vol = root_res_volume;
        Exec_root.res = "OK";
        res_tree.Nodes.push_back(Exec_root);
        return res_tree;

    }
}


void Data_Select_Execute_thread(QTree tree, std::promise<ETree> &resultObj)
{
    QTree sub_tree;
    ETree res_tree;
    QTNode child_node;
    res_tree.tree_id=tree.tree_id;
    res_tree.root=tree.root;
    
    time_t start_time = time(NULL);

    QTNode root=getroot(tree);
    vector<int> children = root.children;
    ETNode Exec_root;
    Exec_root.id = root.id;
    Exec_root.site = root.site;
    Exec_root.children = root.children;
    Exec_root.parent = root.parent;

    //叶节点
    if(tree.Nodes.size() == 1)
    {
        string res_name = "tree_" + to_string(tree.tree_id) + "_node_" + to_string(root.id);
        string select_res;
        if( root.site == LOCALSITE)
        {
            select_res=Select(root.sql_statement, root.site, res_name);
        }
        else
        {
	    std::cout << "root site is: " << root.site << ", but localsite is: " << LOCALSITE << std::endl;
            printf("site do not in this station!");
        }
	std::cout<<"select_res:"<<select_res<<std::endl;
        if(select_res=="FAIL")
        {
            time_t end_time = time(NULL);
            double time_spend = difftime(end_time, start_time);
            Exec_root.time_spend = time_spend;
            Exec_root.vol = 0;
            Exec_root.res = "FAIL";
            res_tree.Nodes.push_back(Exec_root);
            //return res_tree;
            resultObj.set_value(res_tree);
            return;
        }
        else
        {
            string filepath = TMPPATH + res_name + ".sql";
            const char* p = filepath.data();
            //size_t res_volume = get_filebytes(p);
            size_t res_volume = 1;
            time_t end_time = time(NULL);
            double time_spend = difftime(end_time, start_time);
            Exec_root.time_spend = time_spend;
            Exec_root.vol = res_volume;
            Exec_root.res = "OK";
            res_tree.Nodes.push_back(Exec_root);
            //return res_tree;
            resultObj.set_value(res_tree);
            return;
        }
        
    } 
    else
    {
        //非叶节点
        /* 遍历根节点的子节点，获得以该子节点为根节点的子树，这里的遍历先写成循环，通顺以后再改成多线程  */
        /* 先把空间给申请好 */ 
        std::promise<ETree> resultObjs[MAXTHREAD]; 
        std::thread select_threads[MAXTHREAD];
        std::future<ETree> select_thread_exec_trees[MAXTHREAD];
        for(int q=0; q<children.size();q++)
        {
            printf("child %d on processing ...\n", children[q]);
            sub_tree = getsubTree(tree,children[q]);
            child_node = getnode(tree, children[q]);
            int site = child_node.site;

            select_thread_exec_trees[q] = resultObjs[q].get_future();

            if(site == LOCALSITE)
            {
                select_threads[q] = std::thread(Data_Select_Execute_thread, sub_tree, std::ref(resultObjs[q]));
            }
            else
            {
                select_threads[q] = std::thread(RPC_Data_Select_Execute_Thread, sub_tree, site, std::ref(resultObjs[q]));
                printf("remote call in site %d\n", site);
            }
        }

        for(int q=0; q<children.size(); q++)
        {
            string res_name = "tree_" + to_string(tree.tree_id) + "_node_" + to_string(children[q]);
            ETree res_sub_tree = select_thread_exec_trees[q].get();
            ETNode res_sub_root = getroot(res_sub_tree);
            if(res_sub_root.res=="OK")
            {
                for(ETNode tmp_exec_node: res_sub_tree.Nodes)
                {
                    res_tree.Nodes.push_back(tmp_exec_node);
                }
                string tmp_load_res = tmp_load(res_name, root.site);
                if(tmp_load_res == "OK"){
                    printf("tmp table loading of %s succeed!\n", res_name.data());
                }
                else
                {
                    time_t end_time = time(NULL);
                    double time_spend = difftime(end_time, start_time);
                    Exec_root.time_spend = time_spend;
                    Exec_root.vol = 0;
                    Exec_root.res = "FAIL";
                    res_tree.Nodes.push_back(Exec_root);
                    //return res_tree;
                    resultObj.set_value(res_tree);
                    return;
                }
            }
            else
            {
                time_t end_time = time(NULL);
                double time_spend = difftime(end_time, start_time);
                Exec_root.time_spend = time_spend;
                Exec_root.vol = 0;
                Exec_root.res = "FAIL";
                res_tree.Nodes.push_back(Exec_root);
                //return res_tree;
                resultObj.set_value(res_tree);
                return;
            }
        }

        for(int q=0; q<children.size(); q++){
            select_threads[q].join();
        }

    }

    /* 根据根节点的sql_statement执行结果整合 */
    string root_sql = root.sql_statement;
    string root_res_name = "tree_" + to_string(tree.tree_id) + "_node_" + to_string(root.id);
    int root_site = root.site;
    string root_select_res = Select(root_sql, root_site,root_res_name);

    /*
        清理数据库
        算了不想写了
        不行还是得写
     */
    for(int child_id: children){
         string drop_sql = "drop table tree_" + to_string(tree.tree_id) + "_node_" + to_string(child_id);
         string drop_res = Mysql_Delete(drop_sql, root.site);
         if(drop_res != "OK"){
             std::cout<<"release failed! "<< drop_sql.data()<<std::endl;
         }
    }

    if(root_select_res == "FAIL")
    {
        time_t end_time = time(NULL);
        double time_spend = difftime(end_time, start_time);
        Exec_root.time_spend = time_spend;
        Exec_root.vol = 0;
        Exec_root.res = "FAIL";
        res_tree.Nodes.push_back(Exec_root);
        //return res_tree;
        resultObj.set_value(res_tree);
        return;
    }
    else
    {
        string root_filepath = TMPPATH + root_res_name + ".sql";
        const char* root_p = root_filepath.data();
        //size_t root_res_volume = get_filebytes(root_p);
        size_t root_res_volume = 1;

        time_t end_time = time(NULL);
        double time_spend = difftime(end_time, start_time);
        /* 构造本节点的执行记录，合并进执行树 */
        Exec_root.time_spend = time_spend;
        Exec_root.vol = root_res_volume;
        Exec_root.res = "OK";
        res_tree.Nodes.push_back(Exec_root);
        //return res_tree;
        resultObj.set_value(res_tree);
        return;

    }
}
