#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"os.h"
#include"server.h"

//��ʾ����
void help(Client&);
void cmd(Client&);							//�����к���(������������ģ�

//Ӧ�ú���
bool cd_func(Client&, int CurAddr, char* str);
bool mkdir_func(Client&, int CurAddr, char* str);
bool rm_func(Client&, int CurAddr, char* str, char* s_type);
bool touch_func(Client&, int CurAddr, char* str, char* buf);
bool echo_func(Client&, int CurAddr, char* str, char* s_type, char* buf);
bool chmod_func(Client&, int CurAddr, char* pmode, char* str);
bool chown_func(Client&, int CurAddr, char* u_g, char* str);
bool passwd_func(Client&, char* username);