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
	cout.setf(ios::left); //���ö��뷽ʽΪleft 
	cout.width(30); //���ÿ�ȣ������ÿո���� 
	cout << "checkHw" << "View the content of the corresponding assignment " << 
		"(Enter like: checkHw courseName taskName)" <<endl;  //�鿴��ҵ����
	cout.width(30);
	cout << "submit" << "Submit an assignment" << 
		"(Enter like: submit courseName taskName)" <<endl;  //�ύ��ҵ
	cout.width(30);
	cout << "checkScore" << "View the score of the corresponding assignment " << 
		"(Enter like: checkScore courseName taskName)" <<endl;  //�鿴��ҵ����

}


