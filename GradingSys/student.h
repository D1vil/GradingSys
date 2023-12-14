#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"os.h"



//提示函数
void help();

//功能函数
bool ViewAssign(int TAAddr, char* AssignName); //查看作业
bool Submit(int CurAddr, char* AssignName, char* content, char* s_type, char* buf);   //提交作业
bool ViewScore(); //查看作业分数


