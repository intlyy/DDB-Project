# DDB-Project
单机版DDB，在executor里面。 parser里放的是钟锴写的parser模块。

main.cpp:主函数

Tree.h:树结构定义

executor.h:执行模块，其中Client调用Server用Data_Select_Execute，Server之间互相调用用Data_Select_Execute_thread
