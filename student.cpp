#include"os.h"
#include"student.h"
#include<iomanip>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<iostream>

using namespace std;


void help() {
	cout.setf(ios::left); //���ö��뷽ʽΪleft 
	cout.width(30); //���ÿ�ȣ������ÿո���� 
	cout << "ls" << "Display the current directory listing" << endl;	//�г���ǰĿ¼�嵥
	cout.width(30);
	cout << "cd" << "Enter the specific directory" << endl;		//ǰ��ָ��Ŀ¼
	cout.width(30);
	cout << "viewAssign" << "View the content of the corresponding assignment " << endl;  //�鿴��ҵ����
	cout.width(30);
	cout << "submit" << "Submit an assignment" << endl;  //�ύ��ҵ
	cout.width(30);
	cout << "viewScore" << "View the score of the corresponding assignment " << endl;  //�鿴��ҵ����

}


bool Select(int PIAddr, char CourseName[]) {
	mkdir(PIAddr, CourseName);

}