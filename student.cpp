#include"os.h"
#include"student.h"
#include<iomanip>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<iostream>

using namespace std;


void help() {
	cout.setf(ios::left); //设置对齐方式为left 
	cout.width(30); //设置宽度，不足用空格填充 
	cout << "ls" << "Display the current directory listing" << endl;	//列出当前目录清单
	cout.width(30);
	cout << "cd" << "Enter the specific directory" << endl;		//前往指定目录
	cout.width(30);
	cout << "viewAssign" << "View the content of the corresponding assignment " << endl;  //查看作业内容
	cout.width(30);
	cout << "submit" << "Submit an assignment" << endl;  //提交作业
	cout.width(30);
	cout << "viewScore" << "View the score of the corresponding assignment " << endl;  //查看作业分数

}


bool Select(int PIAddr, char CourseName[]) {
	mkdir(PIAddr, CourseName);

}