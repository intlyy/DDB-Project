# DDB-Project
执行主流程在executor里面。 parser里放的是解析sql语句的parser模块。

### Dependency
> grpc  
>   refer:https://xie.infoq.cn/article/5027ae344bf458cdf10e3f633
> etcd-cpp-clientv3   refer: https://github.com/etcd-cpp-apiv3/etcd-cpp-apiv3

### Install

```shell
cd executor
mkdir build && cd build
cmake ..
make
```

## Run
In server site, you need run `server_main` program, and in any site run `main` program, and then input the sql statement

### Notes
main.cpp:主函数

Tree.h:树结构定义

executor.h:执行模块，其中Client调用Server用Data_Select_Execute，Server之间互相调用用Data_Select_Execute_thread
