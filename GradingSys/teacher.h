#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"os.h"


//��ʾ����
void help();


//���ܺ���
bool Assign(int CurAddr, char* AssignName, char* buf, char* s_type);        //������ҵ
bool CheckAssign(int SAAddr, char* str, char* AssignName);  //�鿴ѧ����ҵ
bool Score(int CurAddr, char* FileName, char* content, char* buf, char* s_type);         //���