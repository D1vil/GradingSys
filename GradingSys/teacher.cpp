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
	cout << "Checkteach" << "Display the current directory listing" << endl;	//列出当前目录清单,查看教授课程（ls)
	cout.width(30);
	cout << "assign" << "Post an assignment" << endl;  //布置作业
	cout.width(30);
	cout << "checkAssign" << "Check the corresponding assignment submitted by student" << endl;  //查看学生的作业
	cout.width(30);
	cout << "score" << "Scoring assignment" << endl;  //给作业打分

}





bool Assign(int CurAddr, char* AssignName, char* buf, char* s_type) {//在课程目录下新建作业目录，在作业目录下新建作业文件并写入作业内容到文件中
	mkdir_func(CurAddr, AssignName);
	touch_func(CurAddr, AssignName, buf);
	echo_func(CurAddr, AssignName, s_type, buf);
}

bool CheckAssign(int SAAddr, char* str, char* AssignName) {
	cd_func(SAAddr, str);
	//cat();
}

bool Score(int CurAddr, char* FileName, char* content, char* buf, char* s_type) { //每个作业目录下有每个学生提交作业的目录，里面存了学生的作业文件，教师新建分数文件并写入分数
	touch_func(CurAddr, FileName, buf);
	echo_func(CurAddr, content, s_type, buf);
}