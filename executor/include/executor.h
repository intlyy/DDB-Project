#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include "connect2sql.h"
#include "Tree.h"
#include <thread>
#include <future>

#define LOCALSITE  1 
#define MAXTHREAD 4

ETree Data_Select_Execute(QTree tree);

void Data_Select_Execute_thread(QTree tree, std::promise<ETree> &resultObj);

void RPC_Data_Select_Execute_Thread(QTree tree, int site, std::promise<ETree> &resultObj);
