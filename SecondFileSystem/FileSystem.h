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
	int file_list[1024];//������ļ����
	int file_point[1024];//ָ��
	int load_sb();
	int load_root();
	int load_curnode(int no);
	int show_ls(int no,string name);//ע�⣬no��DiskInode���,����ƫ�Ƶ�ַʱ202*512+no*64
	int show_file(int no);
	int readInode(DiskInode &inode, fstream *disk,int no);
	int IAlloc();//����Inode�ڵ㣬���������Inode�ڵ��
	int Alloc();//������̿�
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

