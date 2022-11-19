#include "executor.h"

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
        if( root.site == LOCALSITE || root.site == LOCALSITE2 )
        {
            select_res=Select(root.sql_statement, root.site, res_name);
        }
        else
        {
            printf("site do not in this station!");
        }
        printf("select_res: %s\n" , select_res);
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

            if(site == LOCALSITE || site == LOCALSITE2)
            {
                select_threads[q] = std::thread(Data_Select_Execute_thread, sub_tree, std::ref(resultObjs[q]));
            }
            else
            {
                printf("WTF???\n");
                //select_threads[q] = std::thread(RPC_Data_Select_Execute_thread, sub_tree, site, std::ref(resultObjs[q]));
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
                string tmp_load_res = tmp_load(res_name, 1);
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
    */

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
        if( root.site == LOCALSITE || root.site == LOCALSITE2 )
        {
            select_res=Select(root.sql_statement, root.site, res_name);
        }
        else
        {
            printf("site do not in this station!");
        }
        printf("select_res: %s\n" , select_res);
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

            if(site == LOCALSITE || site == LOCALSITE2)
            {
                select_threads[q] = std::thread(Data_Select_Execute_thread, sub_tree, std::ref(resultObjs[q]));
            }
            else
            {
                //select_threads[q] = std::thread(RPC_Data_Select_Execute_Thread, sub_tree, site, std::ref(resultObjs[q]));
                printf("WTF???\n");
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
                string tmp_load_res = tmp_load(res_name, 1);
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
    */

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