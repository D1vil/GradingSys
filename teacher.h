#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"os.h"


//提示函数
void help();


//功能函数
bool CheckAssign();
bool Assign(inode fileinode, int PIAddr, char AssignName[], char buf[]);        //布置作业
bool Score(inode fileinode, int PIAddr, char Score[], char buf[]);         //打分