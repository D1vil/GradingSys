#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"os.h"

//��ʾ����
void help();
void cmd(char cmd[], int count);							//�����к���(������������ģ�

//Ӧ�ú���
bool cd_func(int CurAddr, char* str);
bool mkdir_func(int CurAddr, char* str);
bool rm_func(int CurAddr, char* str, char* s_type);
bool touch_func(int CurAddr, char* str, char* buf);