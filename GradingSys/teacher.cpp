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
	cout << "publish [courseName] [taskName]" << "Publish a task" <<endl;  //布置作业
	cout.width(30);
	cout << "judge [courseName] [taskName]" << "Judging homework"<<endl;  //给作业打分

}





