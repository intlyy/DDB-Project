# include "../include/metadata.h"
#include "../include/myetcd.h"
map<string,Fragment> GetETCDFragment(){
    etcd::Client etcd_client(ENDPOINTS);
    vector<string> exist_tables = getTables();
    map<string,Fragment> FragmentMap;
    for(auto &name:exist_tables){
        FragmentMap[name] = getFragFromEtcd(name);
    }
    return FragmentMap;
        
}
vector<string> split2list(string info){
    stringstream sstr(info);
    string token;
    vector<string> res;
    while(getline(sstr,token,',')){
        res.push_back(token);
    }
    return res;
}
vector<string> getTables(){
    //返回创建的所有表格
    etcd::Client etcd_client(ENDPOINTS);
    string key = "/relationdef_table";
    string res  = query_etcd(&etcd_client, key).value().as_string();
    //cout<<res<<endl;
    //string v = get_value_from_json(res);
    vector<string> re = split2list(res);
    return re;
}
vector<string> getTableAttri(etcd::Client* etcd_client, string tableName) {
    //返回表的attri
    string key = "/relationdef_table/"+tableName;
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    vector<string> re = split2list(res);
    return re;
}

string getTableAttriType(etcd::Client* etcd_client, string tableName, string attriName){
    string key = "/relationdef_table/"+tableName+"/"+attriName;
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    return res;
}
string getTableAttriDesc(etcd::Client* etcd_client, string tableName, string attriName){
    string key = "/relationdef_table/"+tableName+"/desc/"+attriName;
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    return res;
}
string getTableKey(string tableName){
    etcd::Client etcd_client(ENDPOINTS);
    string key = "/relationdef_key/"+tableName;
    string res  = query_etcd(&etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    return res;
}
string getTableFragType(etcd::Client* etcd_client, string tableName){
    string key = "/part_schema/"+tableName;
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    return res;
}

vector<string> getTableFragH(etcd::Client* etcd_client, string tableName){
    string key = "/part_schema/"+tableName+"/H";
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    vector<string> re = split2list(res);
    return re;
}
int getTableFragNum(etcd::Client* etcd_client, string tableName){
    string key = "/part_info/"+tableName;
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    char *p = (char*)res.c_str();
    return atoi(p);
}
string getTableFragCondition(etcd::Client* etcd_client, string tableName, int index){
    string key = "/part_info/"+tableName+"/"+tableName+"."+to_string(index);
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    return res;
}

string getTableFragCol(etcd::Client* etcd_client, string tableName, int index){
    string key = "/part_column/"+tableName+"."+to_string(index);
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    return res;
}

int getTableFragSize(etcd::Client* etcd_client, string tableName,int index){
    string key = "/part_size/"+tableName+"."+to_string(index);
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    char *p = (char*)res.c_str();
    return atoi(p);
}
int getTableFragSite(etcd::Client* etcd_client, string tableName,int index){
    string key = "/part_site/"+tableName+"."+to_string(index);
    string res  = query_etcd(etcd_client, key).value().as_string();
    //string v = get_value_from_json(res);
    char *p = (char*)res.c_str();
    return atoi(p);
}


bool saveTableToEtcd(RelationDef table){
    // add table
    etcd::Client etcd_client(ENDPOINTS);
    vector<string> exist_tables = getTables();
    exist_tables.push_back(table.name);
    string k0 = "/relationdef_table";
    string v0 = "";
    // cout << typeid(table).name() << endl;
    // cout << exist_tables.size() << endl;
    for (int i=0; i<exist_tables.size(); i++){
        v0 += exist_tables[i];
        v0 += ",";
    }
    v0.pop_back();
    Insert_Attrvalue(&etcd_client, k0,v0);

    // save table
    string k = "/relationdef_table/"+table.name;
    string v = "";
    // cout << typeid(table).name() << endl;
    // cout << table.cols.size() << endl;
    for (int i=0; i<table.cols.size(); i++){
        v += table.cols[i].name;
        v += ",";
    }
    v.pop_back();
    Insert_Attrvalue(&etcd_client, k,v);

    for (int i=0; i<table.cols.size(); i++){
        ColumnDef col = table.cols[i];
        string k1 = k + "/" + col.name;
        string v1 = col.type;
        Insert_Attrvalue(&etcd_client, k1,v1);

        string k2 = k + "/desc/" + col.name;
        string v2 = col.desc;
        Insert_Attrvalue(&etcd_client, k2,v2);

        if(col.key == true){
            string k3 = "/relationdef_key/"+table.name;
            string v3 = col.name;
            Insert_Attrvalue(&etcd_client, k3,v3);
        }
    }
    return true;
}

RelationDef getTableFromEtcd(string tablename){
    etcd::Client etcd_client(ENDPOINTS);
    RelationDef tbl;
    tbl.name = tablename;
    cout<<"table_name: "<<tablename<<endl;
    string key = getTableKey(tablename);
    cout<<"key: "<<key<<endl;
    vector<string> attri = getTableAttri(&etcd_client, tablename);
    for(int i=0; i<attri.size();i++){
        string att = attri[i];
        ColumnDef col;
        col.name = att;
        cout<<"\tcol_name: "<<col.name<<endl;
        col.type = getTableAttriType(&etcd_client, tablename,att);
        cout<<"\tcol_type: "<<col.type<<endl;
        col.desc = getTableAttriDesc(&etcd_client, tablename,att);
        if(att == "key"){
            col.key=true;
        }
        tbl.cols.push_back(col);
    }
    return tbl;
}

bool saveFragToEtcd(Fragment frag){
    etcd::Client etcd_client(ENDPOINTS);
    string k = "/part_schema/"+frag.name;
    string v = frag.fragtype;
    Insert_Attrvalue(&etcd_client, k,v);

    k = "/part_info/"+frag.name;
    v = to_string(frag.fragnum);
    Insert_Attrvalue(&etcd_client, k,v);

    for (int i=0; i < frag.fragnum; i++){
        FragDef fr = frag.frags[i];
        string k1 = k+"/"+frag.name+"."+to_string(fr.id);
        string v1 = unionvector(fr.condition,"AND");
        Insert_Attrvalue(&etcd_client, k1,v1);

        k1 = "/part_site/"+frag.name+"."+to_string(fr.id);
        v1 = to_string(fr.site);
        Insert_Attrvalue(&etcd_client, k1,v1);

        k1 = "/part_size/"+frag.name+"."+to_string(fr.id);
        v1 = to_string(fr.size);
        Insert_Attrvalue(&etcd_client, k1,v1);

        k1 = "/part_column/"+frag.name+"."+to_string(fr.id);
        v1 = unionvector(fr.column,"AND");
        Insert_Attrvalue(&etcd_client, k1,v1);
    }
    return true;
}

Fragment getFragFromEtcd(string tablename){
    etcd::Client etcd_client(ENDPOINTS);
    Fragment fmt;
    fmt.name = tablename;
    fmt.fragtype = getTableFragType(&etcd_client, tablename);
    fmt.fragnum = getTableFragNum(&etcd_client, tablename);
    cout <<"table_name: "<<tablename<<"\nfragtype: "<<fmt.fragtype<<"\nfmt.fragnum : "<<fmt.fragnum<<endl;
    //cout<<tablename<<endl;
    for(int i=1;i<=fmt.fragnum;i++){
        FragDef df;
        string tmpcol = getTableFragCol(&etcd_client, tablename,i);
        string tmpcondition = getTableFragCondition(&etcd_client, tablename,i);
        //cout<<tmpcondition<<endl;
        splitString(tmpcol,df.column,"AND");
        splitString(tmpcondition,df.condition,"AND");
        fmt.fragcolumns.insert(df.column.begin(),df.column.end());
        df.id = i;
        df.site = getTableFragSite(&etcd_client, tablename,i);
        df.size = getTableFragSize(&etcd_client, tablename,i);
        fmt.frags.push_back(df);
        cout <<"\tfrag_id: "<<i<<"\n\tcondition: "<< unionvector(df.condition,"AND") <<"\n\tcolumns: "<<unionvector(df.column,"AND")<<"\n\tsite: "<<df.site<< endl;
    }
    return fmt;
}

bool Insert_Attrvalue(etcd::Client* etcd_client, string &key,string &value)
{  
    set_etcd(etcd_client, key, value);
    return true; 
}