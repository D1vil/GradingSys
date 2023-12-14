#include"os.h"
#include"teacher.h"
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
	cout << "publish" << "Publish a task" << 
		"(Enter like: publish courseName taskName)" <<endl;  //布置作业
	cout.width(30);
	cout << "judge" << "Judging homework" << 
		"(Enter like: judge courseName taskName)" <<endl;  //给作业打分

}





