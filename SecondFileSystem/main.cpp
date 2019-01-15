/*************************************************************
*姓    名：张宏伟
*学    号：1552238
*班    级：计算机1班
*名    称：二级文件管理系统
*版    本：v0.0.1
*更新时间：20180611
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
	//打开磁盘文件
	//FileSystem *fs=new FileSystem();
	FileSystem fs;
	//fstream disk(DISK, ios::in | ios::out | ios::trunc | ios::binary);
	char cmd_line[256];
	string cmd;//输入命令
	string temp1,temp2;
	stringstream s;
	
	string head = "[HarlanHW@Linux MySFS ";
	string subdir = "~";
	string tail = "]# ";
	int fd;
	fstream demo(DEMO,ios::in);
	while (1)//轮询读入指令
	{
		//读入指令并执行
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
		s.clear();//每次要先清空之类
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
			cout << "help                           帮助     " << endl;
			cout << "mkdir  <dirname>               新建文件夹" << endl;
			cout << "cd     <dirname>               进入文件夹" << endl;
			cout << "ls                             文件列表  " << endl;
			cout << "cat    <filename>              显示文件  " << endl;
			cout << "create <filename> <mode>       新建文件  " << endl;
			cout << "delete <filename>              删除文件  " << endl;
			cout << "open   <filename> <mode>       打开文件  " << endl;
			cout << "close  <fd>                    关闭文件  " << endl;
			cout << "read   <fd> <length>           读文件    " << endl;
			cout << "write  <fd> <buffer> <length>  写文件    " << endl;
			cout << "lseek  <fd> <position>         定位文件  " << endl;
			cout << "exit                           退出程序  " << endl;
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
				fs.fcd(dir_name);//生成文件夹
			else
				cout << "请输入合法文件夹名称" << endl;
		}
		
		if (cmd == "mkdir")
		{
			string dir_name;
			s >> dir_name;
			if (dir_name.size() > 0)
				fs.fmkdir(dir_name);//生成文件夹
			else
				cout << "请输入合法文件夹名称" << endl;
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