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
	cout << "checkHw [courseName] [taskName]" << "View the content of the corresponding assignment " <<endl;  //查看作业内容
	cout.width(30);
	cout << "submit [courseName] [taskName]" << "Submit an assignment" <<endl;  //提交作业
	cout.width(30);
	cout << "checkScore [courseName] [taskName]" << "View the score of the corresponding assignment " <<endl;  //查看作业分数

}


