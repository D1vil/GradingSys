#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"function.h"
#include"os.h"

struct Relation {
	char student[30];
	char lesson[30];
	char teacher[30];
};


bool add_users(char* namelist);			//root������������ʦ��ѧ���û�
bool publish_task(char* lesson, char* filename);	//teacher:����������ҵ����
bool judge_hw(char* namelist, char* lesson, char* hwname);//teacher:���۱�����ҵ


