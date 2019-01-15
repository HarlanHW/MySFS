/*************************************************************
*姓    名：张宏伟
*学    号：1552238
*班    级：计算机1班
*名    称：二级文件管理系统初始化程序
*版    本：v0.0.1
*************************************************************/
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#define DISK "disk.img"
#define DISK_SIZE 4*1024*1024
#define BLOCK_SIZE 512
#define DATA_BEG 1024
using namespace std;
struct Superblock_Temp
{
	int		s_isize;		/* 外存Inode区占用的盘块数 */
	int		s_fsize;		/* 盘块总数 */
	int		s_nfree;		/* 直接管理的空闲盘块数量 */
	int		s_free[100];	/* 直接管理的空闲盘块索引表 */
	int		s_ninode;		/* 直接管理的空闲外存Inode数量 */
	int		s_inode[100];	/* 直接管理的空闲外存Inode索引表 */
	int		s_flock;		/* 封锁空闲盘块索引表标志 */
	int		s_ilock;		/* 封锁空闲Inode表标志 */
	int		s_fmod;			/* 内存中super block副本被修改标志，意味着需要更新外存对应的Super Block */
	int		s_ronly;		/* 本文件系统只能读出 */
	int		s_time;			/* 最近一次更新时间 */
	int		padding[47];	/* 填充使SuperBlock块大小等于1024字节，占据2个扇区 */
};
int main()
{
	/*************************************************************
	*创建磁盘disk.img,大小为DISK_SIZE（4M)
	*无论之前是否有，在初始化时重新创建
	*该程序只在初始化时执行一次
	*************************************************************/		
	fstream disk(DISK, ios::in|ios::out|ios::trunc|ios::binary);
	if (!disk.is_open())
	{
		cout << "创建磁盘disk.img失败" << endl;
		return -1;
	}
	for (int i = 0; i < DISK_SIZE; i++)
	{
		disk <<'\0';
	}
	cout << "创建磁盘成功,磁盘大小" << DISK_SIZE / (1024 * 1024) << "MB..." << endl;
	

	/*************************************************************
	*挂载SuperBlock块
	*************************************************************/
	Superblock_Temp s;
	disk.seekp(200 * BLOCK_SIZE, ios::beg);//此处写入superblock
	s.s_isize = 822;//DiskInode块数
	s.s_fsize = DISK_SIZE / BLOCK_SIZE;//磁盘总块数
	s.s_nfree = (DISK_SIZE/BLOCK_SIZE - 1024 - 99) % 100;//直接管理的块数


	disk.write((char*)(&s.s_isize), sizeof(s.s_isize));
	disk.write((char*)(&s.s_fsize), sizeof(s.s_fsize));
	disk.write((char*)(&s.s_nfree), sizeof(s.s_nfree));

	//管理空闲文件块
	int group_num= (DISK_SIZE/BLOCK_SIZE - 1024 - 99) / 100;//组数
	for (int i = 0; i < s.s_nfree; i++)
	{
		s.s_free[i] = i + 1024;
		disk.write((char*)(&s.s_free[i]), sizeof(s.s_free[i]));
	}
	for (int i = s.s_nfree; i < 100; i++)
	{
		s.s_free[i] = 0;
		disk.write((char*)(&s.s_free[i]), sizeof(s.s_free[i]));
	}

	//管理DiskInode
	//此处所有DiskInode全为空白，但是要把1号留给根目录，所有从2开始数100个
	s.s_ninode = 100;
	disk.write((char*)(&s.s_ninode), sizeof(s.s_ninode));
	for (int i = 2; i < s.s_ninode+2; i++)
	{
		s.s_inode[i] = i;//管理inode索引表
		disk.write((char*)(&s.s_inode[i]), sizeof(s.s_inode[i]));
	}
	
	s.s_flock = 0;
	s.s_ilock = 0;
	s.s_fmod = 0;
	s.s_ronly = 0;
	s.s_time = 0;
	
	disk.write((char*)(&s.s_flock), sizeof(s.s_flock));
	disk.write((char*)(&s.s_ilock), sizeof(s.s_ilock));
	disk.write((char*)(&s.s_fmod), sizeof(s.s_fmod));
	disk.write((char*)(&s.s_ronly), sizeof(s.s_ronly));
	disk.write((char*)(&s.s_time), sizeof(s.s_time));
	
	for (int i = 0; i < 47; i++)
	{
		s.padding[i] = 0;
		disk.write((char*)(&s.padding[i]), sizeof(s.padding[i]));
	}
	cout << "挂载SuperBlock成功..." << endl;
	
	//第n组
	disk.seekp((1024 + 0) * BLOCK_SIZE, ios::beg);
	int num = 100;
	int addr;
	disk.write((char*)(&num), sizeof(num));
	for (int i = 0; i < s.s_nfree; i++)
	{
		addr = (1024 + s.s_nfree + i);
		disk.write((char*)(&addr), sizeof(addr));
	}
	
	for (int i = 0; i < group_num - 1; i++)
	{
		disk.seekp((1024 + s.s_nfree + i * 100) * BLOCK_SIZE, ios::beg);//第n-1组开始
		num = 100;
		disk.write((char*)(&num), sizeof(num));
	
		for (int j = 0; j < 100; j++)
		{
			addr = (1024 + s.s_nfree + 100 * (i + 1) + j);
			disk.write((char*)(&addr), sizeof(addr));
		}
	}
	disk.seekp((1024 + s.s_nfree + (group_num - 1) * 100 + 4) * BLOCK_SIZE, ios::beg);//第1组
	addr = 0;
	disk.write((char*)(&addr), sizeof(addr));
	cout << "初始化空闲块成功..." << endl;
	
	/*************************************************************
	*根目录初始化
	*根目录选择1#DiskInode
	*************************************************************/
	int root_addr;//要新申请一个根目录文件块的地址
	disk.seekp(200 * BLOCK_SIZE + 8, ios::beg);//SuperBlock,读取nfree的值
	int nfree;
	disk.read((char*)(&nfree), sizeof(nfree));
	//cout << nfree;
	root_addr = nfree - 1;//68号
	//cout << root_addr << endl;
	disk.read((char*)(&root_addr), sizeof(root_addr));//直接管理块-1
	root_addr = nfree - 1;//68号,神奇的赋值，不要删
	disk.seekp(200 * BLOCK_SIZE + 12 + 4 * root_addr, ios::beg);
	int sfree = 0;
	disk.write((char*)(&sfree), sizeof(sfree));
	disk.seekp(200 * BLOCK_SIZE + 8, ios::beg);//SuperBlock,读取nfree的值
	nfree -= 1;
	disk.write((char*)(&nfree), sizeof(nfree));

	//


	
	unsigned int d_mode = 0xCDB6;
	int d_link = 1;
	short d_uid = 0;
	short d_gid = 0;
	int d_size = 0;//除了父节点以外没有记录别的
	int d_addr[10];
	//cout << root_addr << endl;
	d_addr[0] = root_addr + DATA_BEG;//根目录文件块的地址

	for (int i = 1; i < 10; i++)
	{
		d_addr[i] = 0;
	}
	int d_atime = 0;
	int d_mtime = 0;
	disk.seekp(202 * BLOCK_SIZE + 64, ios::beg);//1#Inode,开始写
	disk.write((char*)(&d_mode), (sizeof(d_mode)));
	disk.write((char*)(&d_link), (sizeof(d_link)));
	disk.write((char*)(&d_uid), (sizeof(d_uid)));
	disk.write((char*)(&d_gid), (sizeof(d_gid)));
	disk.write((char*)(&d_size), (sizeof(d_size)));
	disk.write((char*)(d_addr), (sizeof(d_addr)));
	disk.write((char*)(&d_atime), (sizeof(d_atime)));
	disk.write((char*)(&d_mtime), (sizeof(d_mtime)));
	//cout << sizeof(d_addr) << endl;

	cout << "初始化根目录DiskInode成功..." << endl;

	disk.seekp(d_addr[0] * BLOCK_SIZE , ios::beg);//根目录数据块
	int cur_node = 1;
	int father_node = 1;
	char cur[28] = ".";
	char father[28] = "..";
	disk.write((char*)(&cur_node), sizeof(cur_node));
	disk.write(cur, sizeof(cur));
	disk.write((char*)(&father_node), sizeof(father_node));
	disk.write(father, sizeof(father));

	cout << "初始化根目录数据块成功..." << endl;
	cout << "二级文件系统初始化!" << endl;
	disk.close();
	return 0;
}