#pragma once
#include"client.h"
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cerrno>
#include<cstdlib>

#define MY_PORT 1234//�˿ں�
#define BUF_SIZE 1024//��󻺴�
#define MAX_QUEUE_NUM 5//���������

char* parseCommand(char* message); // �����û������룬����������������

char* cmd(char* message); // ������ִ�г��򣬷���ִ�н����
// �����messageΪparseCommand�������������ַ���

int run_server();