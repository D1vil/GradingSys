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
	cout << "checkHw [courseName] [taskName]" << "View the content of the corresponding assignment " <<endl;  //�鿴��ҵ����
	cout.width(30);
	cout << "submit [courseName] [taskName]" << "Submit an assignment" <<endl;  //�ύ��ҵ
	cout.width(30);
	cout << "checkScore [courseName] [taskName]" << "View the score of the corresponding assignment " <<endl;  //�鿴��ҵ����

}


