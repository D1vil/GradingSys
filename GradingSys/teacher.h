#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"os.h"


//提示函数
void help();


//功能函数
bool Assign(int CurAddr, char* AssignName, char* buf, char* s_type);        //布置作业
bool CheckAssign(int SAAddr, char* str, char* AssignName);  //查看学生作业
bool Score(int CurAddr, char* FileName, char* content, char* buf, char* s_type);         //打分