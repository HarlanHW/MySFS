/*************************************************************
*��    �����ź�ΰ
*ѧ    �ţ�1552238
*��    ���������1��
*��    �ƣ������ļ�����ϵͳ
*��    ����v0.0.1
*����ʱ�䣺20180611
*************************************************************/
#include <iostream>
#include <sstream>
#include <fstream>
#include "FileSystem.h"
#define DEMO "demo.conf"
using namespace std;
int main(int argc, char * argv[])
{
	cout << "     _________________________________________________________________________ " << endl;
	cout << "    |                      Welcome to My SecondFileSystem                     |" << endl;
	cout << "    |                             Number: 1552238                             |" << endl;
	cout << "    |                             Version: v0.0.1                             |" << endl;
	cout << "    |                          Copyright HarlanHW 2018                        |" << endl;
	cout << "    |_________________________________________________________________________|" << endl;
	cout << "                                                                               " << endl;
	//�򿪴����ļ�
	//FileSystem *fs=new FileSystem();
	FileSystem fs;
	//fstream disk(DISK, ios::in | ios::out | ios::trunc | ios::binary);
	char cmd_line[256];
	string cmd;//��������
	string temp1,temp2;
	stringstream s;
	
	string head = "[HarlanHW@Linux MySFS ";
	string subdir = "~";
	string tail = "]# ";
	int fd;
	fstream demo(DEMO,ios::in);
	while (1)//��ѯ����ָ��
	{
		//����ָ�ִ��
		cout << head.c_str()<<subdir.c_str()<<tail.c_str();
		if (argc > 1)//demo
		{
			if (demo.eof())
			{
				cout << endl;
				return 0;
			}
			demo.getline(cmd_line, 256);
			cout << cmd_line << endl;
			//continue;
		}
		else
		{
			cin.getline(cmd_line, 256);
		}	
		s.clear();//ÿ��Ҫ�����֮��
		cmd = "";
		fd = -1;
		s << cmd_line;
		s >> cmd;
		//cout << cmd << endl;
		if (cmd == "\n")
		{
			continue;
		}
		if (cmd == "help")
		{
			cout << "help                           ����     " << endl;
			cout << "mkdir  <dirname>               �½��ļ���" << endl;
			cout << "cd     <dirname>               �����ļ���" << endl;
			cout << "ls                             �ļ��б�  " << endl;
			cout << "cat    <filename>              ��ʾ�ļ�  " << endl;
			cout << "create <filename> <mode>       �½��ļ�  " << endl;
			cout << "delete <filename>              ɾ���ļ�  " << endl;
			cout << "open   <filename> <mode>       ���ļ�  " << endl;
			cout << "close  <fd>                    �ر��ļ�  " << endl;
			cout << "read   <fd> <length>           ���ļ�    " << endl;
			cout << "write  <fd> <buffer> <length>  д�ļ�    " << endl;
			cout << "lseek  <fd> <position>         ��λ�ļ�  " << endl;
			cout << "exit                           �˳�����  " << endl;
		}
		if (cmd == "exit")
		{
			cout << "Bye~" << endl;
			return 0;

		}
		
		if (cmd == "ls")
		{
			fs.fls();
		}
		
		if (cmd == "cd")
		{
			string dir_name;
			s >> dir_name;
			if (dir_name.size() > 0)
				fs.fcd(dir_name);//�����ļ���
			else
				cout << "������Ϸ��ļ�������" << endl;
		}
		
		if (cmd == "mkdir")
		{
			string dir_name;
			s >> dir_name;
			if (dir_name.size() > 0)
				fs.fmkdir(dir_name);//�����ļ���
			else
				cout << "������Ϸ��ļ�������" << endl;
		}
		
		if (cmd == "create")
		{
			string file_name;
			s >> file_name;
			int mode;
			s >> mode;
			fs.fcreate(file_name, mode);
		}
		
		if (cmd == "delete")
		{
			string file_name;
			s >> file_name;
			//cout << file_name << "aaa" << endl;
			fs.fdelete(file_name);
		}

		if (cmd == "open")
		{
			string file_name;
			s >> file_name;
			int fd = fs.fopen(file_name);
		}
		
		if (cmd == "close")
		{
			int fd;
			s >> fd;
			fs.fclose(fd);
		}
	
		if (cmd == "lseek")
		{
			int fd;
			int pos;
			s >> fd >> pos;
			fs.flseek(fd, pos);
		}
		
		if (cmd == "write")
		{
			int fd;
			string buffer;
			int length;
			s >> fd >> buffer >> length;
			fs.fwrite(fd, buffer, length);
		}

		if (cmd == "read")
		{
			int fd;
			int length;
			string buffer="";
			s >> fd  >> length;
			fs.fread(fd, buffer, length);
			cout << buffer << endl;
		}
		
		if (cmd == "cat")
		{
			
			string name;
			s >> name;
			fs.fcat(name);
		}
	}

	return 0;
}