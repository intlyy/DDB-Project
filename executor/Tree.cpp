#include "./Tree.h"

QTNode getroot(QTree tree)
{
    QTNode root;
    for(int i=0;i<tree.Nodes.size();i++)
    {
        if(tree.Nodes[i].id==tree.root)
        {
            root=tree.Nodes[i];
            return root;
        }
    }
    assert(("QTree root does not exist!"));

}

ETNode getroot(ETree tree)
{
    ETNode root;
    for(int i=0;i<tree.Nodes.size();i++)
    {
        if(tree.Nodes[i].id==tree.root)
        {
            root=tree.Nodes[i];
            return root;
        }
    }
    assert(("ETree root does not exist!"));
}

QTree getsubTree(QTree tree, int  id)
{
    QTree sub_tree;
    sub_tree.tree_id = tree.tree_id;
    sub_tree.root = -1;
    vector<QTNode> sub_nodes;

    for(int i=0; i < tree.Nodes.size(); i++)
    {
        if(tree.Nodes[i].id == id)
        {
            sub_tree.root = id;
            sub_nodes.push_back(tree.Nodes[i]);
            if(tree.Nodes[i].children.size()==0) //叶子节点
            {
                sub_tree.Nodes = sub_nodes;
                return sub_tree;
            }
            else
            {
                vector<int> child_id = tree.Nodes[i].children;
                for(int k=0; k<child_id.size(); k++)
                {
                    QTree child_sub_tree = getsubTree(tree, child_id[k]);
                    for(int j=0;j<child_sub_tree.Nodes.size();j++)
                    {
                        sub_nodes.push_back(child_sub_tree.Nodes[j]);
                    }
                }

                /*
                    去重，应该不会有重复的
                */
                vector<QTNode> sub_nodes_unique;
                int flag=0;
                for(QTNode tmp: sub_nodes)
                {
                    flag=0;
                    for(QTNode tmp2: sub_nodes_unique)
                    {
                        if(tmp.id == tmp2.id)
                        {
                            flag=1;
                            break;
                        }
                    }
                    if(flag==0)
                        sub_nodes_unique.push_back(tmp);
                }
                sub_tree.Nodes = sub_nodes_unique;
                return sub_tree;
            }
        }
    }
    if(sub_tree.root == -1)
    {
        printf("this node does not exist!\n");
        sub_tree.Nodes = sub_nodes;
         return sub_tree;
    }
}

QTNode getnode(QTree tree,int id)
{
    QTNode target;
    target.id = -1;
    for(int i=0; i<tree.Nodes.size();i++)
    {
        if(tree.Nodes[i].id == id)
        {
            target = tree.Nodes[i];
            return target;
        }
    }
    printf("target node does not exist!");
}