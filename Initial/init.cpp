/*************************************************************
*��    �����ź�ΰ
*ѧ    �ţ�1552238
*��    ���������1��
*��    �ƣ������ļ�����ϵͳ��ʼ������
*��    ����v0.0.1
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
	int		s_isize;		/* ���Inode��ռ�õ��̿��� */
	int		s_fsize;		/* �̿����� */
	int		s_nfree;		/* ֱ�ӹ���Ŀ����̿����� */
	int		s_free[100];	/* ֱ�ӹ���Ŀ����̿������� */
	int		s_ninode;		/* ֱ�ӹ���Ŀ������Inode���� */
	int		s_inode[100];	/* ֱ�ӹ���Ŀ������Inode������ */
	int		s_flock;		/* ���������̿��������־ */
	int		s_ilock;		/* ��������Inode���־ */
	int		s_fmod;			/* �ڴ���super block�������޸ı�־����ζ����Ҫ��������Ӧ��Super Block */
	int		s_ronly;		/* ���ļ�ϵͳֻ�ܶ��� */
	int		s_time;			/* ���һ�θ���ʱ�� */
	int		padding[47];	/* ���ʹSuperBlock���С����1024�ֽڣ�ռ��2������ */
};
int main()
{
	/*************************************************************
	*��������disk.img,��СΪDISK_SIZE��4M)
	*����֮ǰ�Ƿ��У��ڳ�ʼ��ʱ���´���
	*�ó���ֻ�ڳ�ʼ��ʱִ��һ��
	*************************************************************/		
	fstream disk(DISK, ios::in|ios::out|ios::trunc|ios::binary);
	if (!disk.is_open())
	{
		cout << "��������disk.imgʧ��" << endl;
		return -1;
	}
	for (int i = 0; i < DISK_SIZE; i++)
	{
		disk <<'\0';
	}
	cout << "�������̳ɹ�,���̴�С" << DISK_SIZE / (1024 * 1024) << "MB..." << endl;
	

	/*************************************************************
	*����SuperBlock��
	*************************************************************/
	Superblock_Temp s;
	disk.seekp(200 * BLOCK_SIZE, ios::beg);//�˴�д��superblock
	s.s_isize = 822;//DiskInode����
	s.s_fsize = DISK_SIZE / BLOCK_SIZE;//�����ܿ���
	s.s_nfree = (DISK_SIZE/BLOCK_SIZE - 1024 - 99) % 100;//ֱ�ӹ���Ŀ���


	disk.write((char*)(&s.s_isize), sizeof(s.s_isize));
	disk.write((char*)(&s.s_fsize), sizeof(s.s_fsize));
	disk.write((char*)(&s.s_nfree), sizeof(s.s_nfree));

	//��������ļ���
	int group_num= (DISK_SIZE/BLOCK_SIZE - 1024 - 99) / 100;//����
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

	//����DiskInode
	//�˴�����DiskInodeȫΪ�հף�����Ҫ��1��������Ŀ¼�����д�2��ʼ��100��
	s.s_ninode = 100;
	disk.write((char*)(&s.s_ninode), sizeof(s.s_ninode));
	for (int i = 2; i < s.s_ninode+2; i++)
	{
		s.s_inode[i] = i;//����inode������
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
	cout << "����SuperBlock�ɹ�..." << endl;
	
	//��n��
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
		disk.seekp((1024 + s.s_nfree + i * 100) * BLOCK_SIZE, ios::beg);//��n-1�鿪ʼ
		num = 100;
		disk.write((char*)(&num), sizeof(num));
	
		for (int j = 0; j < 100; j++)
		{
			addr = (1024 + s.s_nfree + 100 * (i + 1) + j);
			disk.write((char*)(&addr), sizeof(addr));
		}
	}
	disk.seekp((1024 + s.s_nfree + (group_num - 1) * 100 + 4) * BLOCK_SIZE, ios::beg);//��1��
	addr = 0;
	disk.write((char*)(&addr), sizeof(addr));
	cout << "��ʼ�����п�ɹ�..." << endl;
	
	/*************************************************************
	*��Ŀ¼��ʼ��
	*��Ŀ¼ѡ��1#DiskInode
	*************************************************************/
	int root_addr;//Ҫ������һ����Ŀ¼�ļ���ĵ�ַ
	disk.seekp(200 * BLOCK_SIZE + 8, ios::beg);//SuperBlock,��ȡnfree��ֵ
	int nfree;
	disk.read((char*)(&nfree), sizeof(nfree));
	//cout << nfree;
	root_addr = nfree - 1;//68��
	//cout << root_addr << endl;
	disk.read((char*)(&root_addr), sizeof(root_addr));//ֱ�ӹ����-1
	root_addr = nfree - 1;//68��,����ĸ�ֵ����Ҫɾ
	disk.seekp(200 * BLOCK_SIZE + 12 + 4 * root_addr, ios::beg);
	int sfree = 0;
	disk.write((char*)(&sfree), sizeof(sfree));
	disk.seekp(200 * BLOCK_SIZE + 8, ios::beg);//SuperBlock,��ȡnfree��ֵ
	nfree -= 1;
	disk.write((char*)(&nfree), sizeof(nfree));

	//


	
	unsigned int d_mode = 0xCDB6;
	int d_link = 1;
	short d_uid = 0;
	short d_gid = 0;
	int d_size = 0;//���˸��ڵ�����û�м�¼���
	int d_addr[10];
	//cout << root_addr << endl;
	d_addr[0] = root_addr + DATA_BEG;//��Ŀ¼�ļ���ĵ�ַ

	for (int i = 1; i < 10; i++)
	{
		d_addr[i] = 0;
	}
	int d_atime = 0;
	int d_mtime = 0;
	disk.seekp(202 * BLOCK_SIZE + 64, ios::beg);//1#Inode,��ʼд
	disk.write((char*)(&d_mode), (sizeof(d_mode)));
	disk.write((char*)(&d_link), (sizeof(d_link)));
	disk.write((char*)(&d_uid), (sizeof(d_uid)));
	disk.write((char*)(&d_gid), (sizeof(d_gid)));
	disk.write((char*)(&d_size), (sizeof(d_size)));
	disk.write((char*)(d_addr), (sizeof(d_addr)));
	disk.write((char*)(&d_atime), (sizeof(d_atime)));
	disk.write((char*)(&d_mtime), (sizeof(d_mtime)));
	//cout << sizeof(d_addr) << endl;

	cout << "��ʼ����Ŀ¼DiskInode�ɹ�..." << endl;

	disk.seekp(d_addr[0] * BLOCK_SIZE , ios::beg);//��Ŀ¼���ݿ�
	int cur_node = 1;
	int father_node = 1;
	char cur[28] = ".";
	char father[28] = "..";
	disk.write((char*)(&cur_node), sizeof(cur_node));
	disk.write(cur, sizeof(cur));
	disk.write((char*)(&father_node), sizeof(father_node));
	disk.write(father, sizeof(father));

	cout << "��ʼ����Ŀ¼���ݿ�ɹ�..." << endl;
	cout << "�����ļ�ϵͳ��ʼ��!" << endl;
	disk.close();
	return 0;
}