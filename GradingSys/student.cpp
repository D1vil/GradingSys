#include"os.h"
#include"student.h"
#include"function.h"
#include<iomanip>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<iostream>

using namespace std;


void help() {
	cout.setf(ios::left); //设置对齐方式为left 
	cout.width(30); //设置宽度，不足用空格填充 
	cout << "CheckCourse" << "Display the current directory listing" << endl;	//列出当前目录清单,查看当前选修课程
	cout.width(30);
	cout << "viewAssign" << "View the content of the corresponding assignment " << endl;  //查看作业内容
	cout.width(30);
	cout << "submit" << "Submit an assignment" << endl;  //提交作业
	cout.width(30);
	cout << "viewScore" << "View the score of the corresponding assignment " << endl;  //查看作业分数

}


bool ViewAssign(int TAAddr, char* AssignName) {
	cd_func(TAAddr, AssignName);
	//cat();
}

bool submit(int CurAddr, char* AssignName,char* content, char* s_type, char* buf) {
	mkdir_func(CurAddr, AssignName);
	touch_func(CurAddr + / AssignName, AssignName, buf);
	echo_func(CurAddr + / AssignName, content, s_type, buf);
}

bool ViewScore(int TSAddr, char* AssignName) {
	cd_func(TSAddr, AssignName);
	//cat();
}