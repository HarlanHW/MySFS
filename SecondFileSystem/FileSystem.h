#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include "SuperBlock.h"
#include "INode.h"
#define DISK "disk.img"
#define DISK_SIZE 4*1024*1024
#define BLOCK_SIZE 512
#define DATA_BEG 1024
#define INODE_SIZE 64
using namespace std;

class FileSystem
{	
public:
	fstream *disk;
	SuperBlock sb;
	DiskInode root_node;
	DiskInode cur_node;
	int file_list[1024];//管理的文件句柄
	int file_point[1024];//指针
	int load_sb();
	int load_root();
	int load_curnode(int no);
	int show_ls(int no,string name);//注意，no是DiskInode编号,计算偏移地址时202*512+no*64
	int show_file(int no);
	int readInode(DiskInode &inode, fstream *disk,int no);
	int IAlloc();//申请Inode节点，返回申请的Inode节点号
	int Alloc();//申请磁盘块
	int IFree(int no);
	int writeInode(DiskInode inode,fstream *disk,int inode_num);
	
	FileSystem();
	~FileSystem();
	int fls();
	int fmkdir(string dir_name);
	int fcd(string dir_name);
	int fcreate(string filename,int mode);
	int fopen(string filename);
	int fclose(int fd);
	int fread(int fd, string &buffer, int length);
	int fwrite(int fd,string buffer,int length);
	
	int flseek(int fd,int position);
	int fdelete(string filename);
	int fcat(string filename);
	
	char* get_path();

};

