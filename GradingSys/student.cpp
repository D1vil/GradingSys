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
	cout << "CheckCourse" << "Display the current directory listing" << endl;	//�г���ǰĿ¼�嵥,�鿴��ǰѡ�޿γ�
	cout.width(30);
	cout << "viewAssign" << "View the content of the corresponding assignment " << endl;  //�鿴��ҵ����
	cout.width(30);
	cout << "submit" << "Submit an assignment" << endl;  //�ύ��ҵ
	cout.width(30);
	cout << "viewScore" << "View the score of the corresponding assignment " << endl;  //�鿴��ҵ����

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