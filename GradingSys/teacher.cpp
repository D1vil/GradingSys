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
	cout.setf(ios::left); //���ö��뷽ʽΪleft 
	cout.width(30); //���ÿ�ȣ������ÿո���� 
	cout << "publish [courseName] [taskName]" << "Publish a task" <<endl;  //������ҵ
	cout.width(30);
	cout << "judge [courseName] [taskName]" << "Judging homework"<<endl;  //����ҵ���

}





