#include "../include/connect2sql.h"
//g++ connect2sql.cpp -o testselect -lmysqlclient

/*
int main()
{
    //string sql="insert into test values('user2',123456)";
   // std::cout<<Insert(sql,"s1");
    //string sql="select * from test.test";
    //std::cout<<Select(sql,1,"testselect");
    res_print("testselect");
}
*/


auto Select_for_load(string sql,int site,string res_name,int &row_num) ->string
{
    char* DATABASE="test";
    char* DATABASE1="test";
    char* DATABASE2="remot_test";
    MYSQL mysql;
    int res=-1;
    int PORT=-1;
    const char* SOCKET;

    if(site==2)
    {
        PORT = 7655;
        SOCKET = "/var/run/mysqld/mysqld.sock";
        //DATABASE = DATABASE2;
    }
    else
    {
        PORT = 7654;
        SOCKET = "/var/run/mysqld/mysqld.sock";
        //DATABASE = DATABASE1;
    }        
    
            
    assert(("非法site",PORT!=-1));

    mysql_init(&mysql);
    if(mysql_real_connect(&mysql,HOST,USERNAME,PASSWORD,DATABASE,PORT,SOCKET,0))
    {
        printf("connect success!\n");
        /*这句话是设置查询编码为utf8，这样支持中文*/
        mysql_query(&mysql, "set names utf8");

        string new_sql = "create table ";
        new_sql.append(res_name);
        new_sql.append(" ");
        new_sql.append(sql);
        const char* p = new_sql.data();
        /* 执行 */
        res = mysql_query(&mysql, p);
        row_num = mysql_affected_rows(&mysql);

        if (res) 
        {   /*执行失败了*/
            printf("Error: mysql_query !\n");

            std::cout<< new_sql<<endl;
            cout<<mysql_error(&mysql)<<endl;
            return "FAIL";
        }
        else 
        { 
            /*现在就代表执行成功了*/
            /* 构建cmd要执行的导出文件命令 */
            /* 举例：mysqldump -u root -S /home/mysql1/mysql.sock -prootroot test book > book.sql */
            string command_save = "mysqldump -u ";
            command_save.append(USERNAME);
            command_save.append(" -S ");
            command_save.append(SOCKET);
            //command_save.append(" -p");
            //command_save.append(PASSWORD);
            command_save.append(" ");
            command_save.append(DATABASE);
            command_save.append(" ");
            command_save.append(res_name);
            command_save.append(" > ");
            command_save.append(TMPPATH);
            command_save.append(res_name);
            command_save.append(".sql");
            /* 执行 */
            std::cout<<command_save<<std::endl;
            const char* q = command_save.data();
            int tmp=system(q);  
            assert(("system error!",tmp!=-1 && tmp!=127));
            std::cout<<"tmp:"<<tmp<<std::endl;
            /* 构建语句删除原来的表 */
            string new_sql = "drop table ";
            new_sql.append(res_name);
            const char* p = new_sql.data();
            /* 执行 */
            res = mysql_query(&mysql, p);
            
            if (res) 
            {   /*现在就代表执行失败了*/
                printf("Error: mysql release error!\n");
            }
            else 
            { 
                /*现在就代表执行成功了*/
            }
            mysql_close(&mysql);
            return res_name;
        }             
    }
    else
    {
        printf("connect error!");
        return "FAIL";
    }
    
}

auto Insert(string sql,int site) ->string
{
    MYSQL mysql;
    int res=-1;
    int PORT=-1;
    char* DATABASE="test";
    const char* SOCKET;

    if(site==4)
    {
        PORT = 7655;
        SOCKET = "/var/run/mysqld/mysqld.sock";
    }
    else
    {
        PORT = 7654;
        SOCKET = "/var/run/mysqld/mysqld.sock";
    }        
    
            
    assert(("非法site",PORT!=-1));

    mysql_init(&mysql);
    if(mysql_real_connect(&mysql,HOST,USERNAME,PASSWORD,DATABASE,PORT,SOCKET,0))
    {
        printf("connect success!\n");
        const char* p = sql.data();
        res=mysql_query(&mysql,p);
        mysql_close(&mysql);
        if(res)
        {
            printf("error %d !\n",res);
            return "FAILED";
        }
        else
        {
            printf("success!\n");
            return "success";
        }
    }
    else
    {
        printf("connect failed\n");
    }
    assert(res!=-1);

}

auto Delete(string sql,int site) ->string;
{
    MYSQL mysql;
    int res=-1;
    int PORT=-1;
    char* DATABASE="test";
    const char* SOCKET;

    if(site==2)
    {
        PORT = 7655;
        SOCKET = "/var/run/mysqld/mysqld.sock";
    }
    else
    {
        PORT = 7654;
        SOCKET = "/var/run/mysqld/mysqld.sock";
    }        
    
            
    assert(("非法site",PORT!=-1));

    mysql_init(&mysql);
    if(mysql_real_connect(&mysql,HOST,USERNAME,PASSWORD,DATABASE,PORT,SOCKET,0))
    {
        printf("connect success!\n");
        const char* p = sql.data();
        res=mysql_query(&mysql,p);
        mysql_close(&mysql);
        if(res)
        {
            printf("error %d !\n",res);
            return "FAILED";
        }
        else
        {
            printf("success!\n");
            return "success";
        }
    }
    else
    {
        printf("connect failed\n");
    }
    assert(res!=-1);

}


auto Select(string sql,int site,string res_name) ->string
{
    char* DATABASE="test";
    char* DATABASE1="test";
    char* DATABASE2="remot_test";
    MYSQL mysql;
    int res=-1;
    int PORT=-1;
    const char* SOCKET;

    if(site==2)
    {
        PORT = 7655;
        SOCKET = "/var/run/mysqld/mysqld.sock";
    }
    else
    {
        PORT = 7654;
        SOCKET = "/var/run/mysqld/mysqld.sock";
    }        
    
            
    assert(("非法site",PORT!=-1));

    mysql_init(&mysql);
    if(mysql_real_connect(&mysql,HOST,USERNAME,PASSWORD,DATABASE,PORT,SOCKET,0))
    {
        printf("connect success!\n");
        /*这句话是设置查询编码为utf8，这样支持中文*/
        mysql_query(&mysql, "set names utf8");

        string new_sql = "create table ";
        new_sql.append(res_name);
        new_sql.append(" ");
        new_sql.append(sql);
        const char* p = new_sql.data();
        /* 执行 */
        res = mysql_query(&mysql, p);

        if (res) 
        {   /*执行失败了*/
            printf("Error: mysql_query !\n");
            std::cout<< new_sql<<endl;
            cout<<mysql_error(&mysql)<<endl;
            return "FAIL";
        }
        else 
        { 
            /*现在就代表执行成功了*/
            /* 构建cmd要执行的导出文件命令 */
            /* 举例：mysqldump -u root -S /home/mysql1/mysql.sock -prootroot test book > book.sql */
            string command_save = "mysqldump -u ";
            command_save.append(USERNAME);
            command_save.append(" -S ");
            command_save.append(SOCKET);
            command_save.append(" -p");
            command_save.append(PASSWORD);
            command_save.append(" ");
            command_save.append(DATABASE);
            command_save.append(" ");
            command_save.append(res_name);
            command_save.append(" > ");
            command_save.append(TMPPATH);
            command_save.append(res_name);
            command_save.append(".sql");
            /* 执行 */
            std::cout<<command_save<<std::endl;
            const char* q = command_save.data();
            int tmp=system(q);  
            assert(("system error!",tmp!=-1 && tmp!=127));
            std::cout<<"tmp:"<<tmp<<std::endl;
            /* 构建语句删除原来的表 */
            string new_sql = "drop table ";
            new_sql.append(res_name);
            const char* p = new_sql.data();
            /* 执行 */
            res = mysql_query(&mysql, p);
            
            if (res) 
            {   /*现在就代表执行失败了*/
                printf("Error: mysql release error!\n");
            }
            else 
            { 
                /*现在就代表执行成功了*/
            }
            mysql_close(&mysql);
            return res_name;
        }             
    }
    else
    {
        printf("connect error!");
        return "FAIL";
    }
    
}

void res_print(string my_res_name)
{
    char* DATABASE="test";
    /* 将sql文件读入MySQL */
    MYSQL conn;
    int res;
    int res_load;
    MYSQL_RES *res_ptr; /*指向查询结果的指针*/
    MYSQL_FIELD *field; /*字段结构指针*/
    MYSQL_ROW result_row; /*按行返回的查询信息*/

    int row, column; /*查询返回的行数和列数*/
    int i, j; /*只是控制循环的两个变量*/

    int PORT;
    const char* UNIX_SOCKET;

    /* 现在直接默认使用第一个连接 */
    PORT = 7654;
    UNIX_SOCKET = "/var/run/mysqld/mysqld.sock";

    /* 构建cmd要执行的导入文件命令 */
    /* 举例：mysql -u root -S /home/mysql2/mysql.sock -prootroot -Dtest < /mnt/d/ddbms200tmp/tmp_book_1.sql */
    string command_save = "mysql -u ";
    command_save.append(USERNAME);
    command_save.append(" -S ");
    command_save.append(UNIX_SOCKET);
    command_save.append(" -p");
    command_save.append(PASSWORD);
    command_save.append(" -D");
    command_save.append(DATABASE);
    command_save.append(" ");
    command_save.append(" < ");
    command_save.append(TMPPATH);
    command_save.append(my_res_name);
    command_save.append(".sql");
    /* 执行 */
    const char* q = command_save.data();
    int tmp=system(q);  //此处应当判断一下成功没有，但是我没写
    assert(("system error!",tmp==0));

    mysql_init(&conn);
    if(mysql_real_connect(&conn, HOST, USERNAME, PASSWORD, DATABASE, PORT, UNIX_SOCKET,0))
    {
        printf("connect success!\n");
        /* 构建取表语句 */
        string sql_print = "select * from ";
        sql_print.append(my_res_name);
        /* 执行取表语句 */
        const char* p = sql_print.data();
        res_load=mysql_query(&conn,p);
        if(res_load){
            /* 取数据表失败 */
            printf("DATA READ FAILED\n");
        }
        else /* 取数据成功 */
        {
            /*将查询的結果给res_ptr*/
            res_ptr = mysql_store_result(&conn);
            /*如果结果不为空，就把结果print*/
            if (res_ptr) 
            {
                /*取得結果的行列数*/
                column = mysql_num_fields(res_ptr);
                row = mysql_num_rows(res_ptr);
                printf("查询到 %d 行 \n", row);

                /*输出結果的字段名*/
                for (i = 0; field = mysql_fetch_field(res_ptr); i++)
                {
                    printf("%s\t", field->name);
                }
                    
                printf("\n");

                /*按行输出結果*/
                for (i = 1; i <= row; i++)
                {
                    result_row = mysql_fetch_row(res_ptr);
                    for (j = 0; j < column; j++)
                    {
                        printf("%s\t", result_row[j]);
                    }
                    printf("\n"); 
            
                }
            }
            else
            {
                printf("Empty!");
            }
            /* 构建语句删除原来的表 */
            string new_sql = "drop table ";
            new_sql.append(my_res_name);
            const char* p = new_sql.data();
            /* 执行 */
            res = mysql_query(&conn, p);
        
            if (res) 
            {   /*现在就代表执行失败了*/
                printf("Error： mysql release error!\n");
            }
            else 
            { 
                /*现在就代表执行成功了*/
            }
            /* 关闭连接 */
            mysql_close(&conn);
            mysql_free_result(res_ptr);
        }
    }
            
}
auto Load(string sql_create,string sql_load,int site) ->string;

auto tmp_load(string tmp_data,int site) ->string
{
    char* DATABASE="test";
    int PORT;
    const char* UNIX_SOCKET;

    /* 通过站点名称判断使用哪个连接 */
    if(site == 4){
        PORT = 7655;
        UNIX_SOCKET = "/var/run/mysqld/mysqld.sock";
    }
    else{
        PORT = 7654;
        UNIX_SOCKET = "/var/run/mysqld/mysqld.sock";
    } // 此处如果输入了没出现过的站点应当报警但我没写

    
    /* 构建cmd要执行的导入文件命令 */
    /* 举例：mysql -u root -S /home/mysql2/mysql.sock -prootroot -Dtest < /mnt/d/ddbms200tmp/tmp_book_1.sql */
    string command_save = "mysql -u ";
    command_save.append(USERNAME);
    command_save.append(" -S ");
    command_save.append(UNIX_SOCKET);
    command_save.append(" -p");
    command_save.append(PASSWORD);
    command_save.append(" -D");
    command_save.append(DATABASE);
    command_save.append(" ");
    command_save.append(" < ");
    command_save.append(TMPPATH);
    command_save.append(tmp_data);
    command_save.append(".sql");
    /* 执行 */
    const char* q = command_save.data();
    int tmp=system(q);  //此处应当判断一下成功没有，但是我没写
    assert(("system error!",tmp==0));
    return "OK";
}

auto Mysql_Delete(string sql, int site) ->string
{
     char* DATABASE="test";
     char* DATABASE1="test";
     char* DATABASE2="remot_test";
     MYSQL mysql;
     int res=-1;
     int PORT=-1;
     const char* SOCKET;

     if(site==2)
     {
         PORT = 7655;
         SOCKET = "/var/run/mysqld/mysqld.sock";
     }
     else
     {
         PORT = 7654;
         SOCKET = "/var/run/mysqld/mysqld.sock";
     }        


     assert(("非法site",PORT!=-1));

     mysql_init(&mysql);
     if(mysql_real_connect(&mysql,HOST,USERNAME,PASSWORD,DATABASE,PORT,SOCKET,0))
     {
         printf("connect success to delete!\n");
         const char* p = sql.data();
         res=mysql_query(&mysql,p);
         mysql_close(&mysql);
         if(res)
         {
             printf("error %d !\n",res);
             return "FAILED";
         }
         else
         {
             printf("delete OK\n");
             return "OK";
         }
     }
     else
     {
         printf("connect failed\n");
     }
     assert(res!=-1);
}


auto res_print(string res_name) ->void;

auto res_getrows(string res_name) ->int;

vector<int> res_getcol1(string res_name); 

//update user set password=password('root') where user='root' and host='localhost' qyjmuy56xTKsYKnG
