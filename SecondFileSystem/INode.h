#ifndef INODE_H
#define INODE_H

/*
* 内存索引节点(INode)的定义
* 系统中每一个打开的文件、当前访问目录、
* 挂载的子文件系统都对应唯一的内存inode。
* 每个内存inode通过外存inode所在存储设备的设备号(i_dev)
* 以及该设备外存inode区中的编号(i_number)来确定
* 其对应的外存inode。
*/


/*
* 外存索引节点(DiskINode)的定义
* 外存Inode位于文件存储设备上的
* 外存Inode区中。每个文件有唯一对应
* 的外存Inode，其作用是记录了该文件
* 对应的控制信息。
* 外存Inode中许多字段和内存Inode中字段
* 相对应。外存INode对象长度为64字节，
* 每个磁盘块可以存放512/64 = 8个外存Inode
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
	unsigned int d_mode;	/* 状态的标志位，定义见enum INodeFlag */
	int		d_nlink;		/* 文件联结计数，即该文件在目录树中不同路径名的数量 */

	short	d_uid;			/* 文件所有者的用户标识数 */
	short	d_gid;			/* 文件所有者的组标识数 */

	int		d_size;			/* 文件大小，字节为单位 */
	int		d_addr[10];		/* 用于文件逻辑块好和物理块好转换的基本索引表 */

	int		d_atime;		/* 最后访问时间 */
	int		d_mtime;		/* 最后修改时间 */
};

#endif
