#include "INode.h"
/*============================class DiskInode=================================*/

DiskInode::DiskInode()
{
	/*
	* ���DiskInodeû�й��캯�����ᷢ�����½��Ѳ���Ĵ���
	* DiskInode��Ϊ�ֲ�����ռ�ݺ���Stack Frame�е��ڴ�ռ䣬����
	* ��οռ�û�б���ȷ��ʼ�����Ծɱ�������ǰջ���ݣ����ڲ�����
	* DiskInode�����ֶζ��ᱻ���£���DiskInodeд�ص�������ʱ������
	* ����ǰջ����һͬд�أ�����д�ؽ������Ī����������ݡ�
	*/
	this->d_mode = 0;
	this->d_nlink = 0;
	this->d_uid = -1;
	this->d_gid = -1;
	this->d_size = 0;
	for (int i = 0; i < 10; i++)
	{
		this->d_addr[i] = 0;
	}
	this->d_atime = 0;
	this->d_mtime = 0;
}

DiskInode::~DiskInode()
{
	//nothing to do here
}
