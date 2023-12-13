#include"os.h"
#include"teacher.h"
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
	cout << "assign" << "Post an assignment" << endl;  //布置作业
	cout.width(30);
	cout << "checkAssign" << "Check the corresponding assignment submitted by student" << endl;  //查看学生的作业
	cout.width(30);
	cout << "score" << "Scoring assignment" << endl;  //给作业打分

}





bool Assign(inode fileinode, int PIAddr, char AssignName[], char buf[]) {//在课程目录下新建作业目录，在作业目录下新建作业文件并写入作业内容到文件中
	mkdir(PIAddr, AssignName);
	mkfile(PIAddr + / AssignName, AssignName, buf);
	writefile(fileinode, PIAddr + / AssignName, buf);

}

bool Score(inode fileinode, int PIAddr, char Score[], char buf[]) { //每个作业目录下有每个学生提交作业的目录，里面存了学生的作业文件，教师新建分数文件并写入分数
	mkfile(PIAddr, Score, buf);
	writefile(fileinode, PIAddr, buf);
}