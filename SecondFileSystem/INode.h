#ifndef INODE_H
#define INODE_H

/*
* �ڴ������ڵ�(INode)�Ķ���
* ϵͳ��ÿһ���򿪵��ļ�����ǰ����Ŀ¼��
* ���ص����ļ�ϵͳ����ӦΨһ���ڴ�inode��
* ÿ���ڴ�inodeͨ�����inode���ڴ洢�豸���豸��(i_dev)
* �Լ����豸���inode���еı��(i_number)��ȷ��
* ���Ӧ�����inode��
*/


/*
* ��������ڵ�(DiskINode)�Ķ���
* ���Inodeλ���ļ��洢�豸�ϵ�
* ���Inode���С�ÿ���ļ���Ψһ��Ӧ
* �����Inode���������Ǽ�¼�˸��ļ�
* ��Ӧ�Ŀ�����Ϣ��
* ���Inode�������ֶκ��ڴ�Inode���ֶ�
* ���Ӧ�����INode���󳤶�Ϊ64�ֽڣ�
* ÿ�����̿���Դ��512/64 = 8�����Inode
*/
class DiskInode
{
	/* Functions */
public:
	/* Constructors */
	DiskInode();
	/* Destructors */
	~DiskInode();

	/* Members */
public:
	unsigned int d_mode;	/* ״̬�ı�־λ�������enum INodeFlag */
	int		d_nlink;		/* �ļ���������������ļ���Ŀ¼���в�ͬ·���������� */

	short	d_uid;			/* �ļ������ߵ��û���ʶ�� */
	short	d_gid;			/* �ļ������ߵ����ʶ�� */

	int		d_size;			/* �ļ���С���ֽ�Ϊ��λ */
	int		d_addr[10];		/* �����ļ��߼���ú��������ת���Ļ��������� */

	int		d_atime;		/* ������ʱ�� */
	int		d_mtime;		/* ����޸�ʱ�� */
};

#endif