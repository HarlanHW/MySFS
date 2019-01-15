#include "FileSystem.h"

int FileSystem::IAlloc()
{
	int inode_num=-1;
	if (sb.s_ninode > 0)//有Inode可用
	{
		int inode_num = sb.s_inode[sb.s_ninode - 1];//用最后一块
		sb.s_inode[sb.s_ninode - 1] = 0;
		sb.s_ninode -= 1;
		return inode_num;
	}
	else
		return -1;//再找100个Inode来
	
}

int FileSystem::Alloc()
{
	int block_num=-1;
	if (sb.s_nfree > 1)//有块可用
	{
		block_num = sb.s_free[sb.s_nfree - 1];
		sb.s_free[sb.s_nfree - 1] = 0;
		sb.s_nfree -= 1;
		return block_num;
	}
	else
		return -1;
	
}

int FileSystem::IFree(int no)
{
	disk->seekp(202 * BLOCK_SIZE + no * INODE_SIZE, ios::beg);
	unsigned int d_mode = 0;
	for (int i = 0; i < 16; i++)
	{
		disk->write((char*)(&d_mode), sizeof(d_mode));
	}
	
	return 0;
}

int FileSystem::writeInode(DiskInode inode, fstream *disk, int inode_num)
{
	disk->seekp(202 * BLOCK_SIZE + inode_num * INODE_SIZE, ios::beg);
	disk->write((char*)(&inode.d_mode),sizeof(inode.d_mode));
	disk->write((char*)(&inode.d_nlink), sizeof(inode.d_nlink));
	disk->write((char*)(&inode.d_uid), sizeof(inode.d_uid));
	disk->write((char*)(&inode.d_gid), sizeof(inode.d_gid));
	disk->write((char*)(&inode.d_size), sizeof(inode.d_size));
	disk->write((char*)(inode.d_addr), sizeof(inode.d_addr));
	disk->write((char*)(&inode.d_atime), sizeof(inode.d_atime));
	disk->write((char*)(&inode.d_mtime), sizeof(inode.d_mtime));
	//cout << inode.d_mode << endl;
	return 0;
}

int FileSystem::load_sb()
{
	disk->seekg(200 * BLOCK_SIZE, ios::beg);

	disk->read((char*)(&sb.s_isize), sizeof(sb.s_isize));
	disk->read((char*)(&sb.s_fsize), sizeof(sb.s_fsize));
	disk->read((char*)(&sb.s_nfree), sizeof(sb.s_nfree));
	disk->read((char*)(sb.s_free), sizeof(sb.s_free));


	disk->read((char*)(&sb.s_ninode), sizeof(sb.s_ninode));
	disk->read((char*)(sb.s_inode), sizeof(sb.s_inode));

	disk->read((char*)(&sb.s_flock), sizeof(sb.s_flock));
	disk->read((char*)(&sb.s_ilock), sizeof(sb.s_ilock));
	disk->read((char*)(&sb.s_fmod), sizeof(sb.s_fmod));
	disk->read((char*)(&sb.s_ronly), sizeof(sb.s_ronly));
	disk->read((char*)(&sb.s_time), sizeof(sb.s_time));
	disk->read((char*)(sb.padding), sizeof(sb.padding));

	//cout << sb.s_nfree << endl;
	return 0;
}

int FileSystem::load_root()
{
	
	disk->seekg(202 * BLOCK_SIZE + 64, ios::beg);
	disk->read((char*)(&root_node.d_mode), sizeof(root_node.d_mode));
	disk->read((char*)(&root_node.d_nlink), sizeof(root_node.d_nlink));
	disk->read((char*)(&root_node.d_uid), sizeof(root_node.d_uid));
	disk->read((char*)(&root_node.d_gid), sizeof(root_node.d_gid));
	disk->read((char*)(&root_node.d_size), sizeof(root_node.d_size));
	disk->read((char*)(root_node.d_addr), sizeof(root_node.d_addr));
	disk->read((char*)(&root_node.d_atime), sizeof(root_node.d_atime));
	disk->read((char*)(&root_node.d_mtime), sizeof(root_node.d_mtime));
	//cout << root_node.d_size << endl;

	return 0;
}

int FileSystem::load_curnode(int no)
{
	disk->seekg(no*BLOCK_SIZE, ios::beg);
	int cur_no;
	disk->read((char*)(&cur_no), sizeof(cur_no));
	disk->seekg(202 * BLOCK_SIZE + cur_no * INODE_SIZE, ios::beg);
	disk->read((char*)(&cur_node), sizeof(cur_node));
	//cout << sizeof(cur_node) << endl;
	cout << cur_node.d_addr[0] << endl;
	return 0;
}

int FileSystem::show_ls(int no,string name)
{
	disk->seekg(202 * BLOCK_SIZE + no * INODE_SIZE, ios::beg);
	DiskInode inode;
	disk->read((char*)(&inode.d_mode), sizeof(inode.d_mode));
	//cout << cur_node.d_mode;
	//cout << inode.d_mode << endl;
	if (inode.d_mode & 0x4000)//目录文件
		cout << "d";
	else
		cout << "-";
	
	//权限信息
	if (inode.d_mode & 0x0100)//普通文件
		cout << "r";
	else
		cout << "-";
	if (inode.d_mode & 0x0080)//普通文件
		cout << "w";
	else
		cout << "-";
	if (inode.d_mode & 0x0040)//普通文件
		cout << "x";
	else
		cout << "-";

	if (inode.d_mode & 0x0020)//普通文件
		cout << "r";
	else
		cout << "-";
	if (inode.d_mode & 0x0010)//普通文件
		cout << "w";
	else
		cout << "-";
	if (inode.d_mode & 0x0008)//普通文件
		cout << "x";
	else
		cout << "-";

	if (inode.d_mode & 0x0004)//普通文件
		cout << "r";
	else
		cout << "-";
	if (inode.d_mode & 0x0002)//普通文件
		cout << "w";
	else
		cout << "-";
	if (inode.d_mode & 0x0001)//普通文件
		cout << "x";
	else
		cout << "-";

	cout << " ";

	//cur_node.d_uid; cur_node.d_gid;
	//可以直接读取uid和gid，但是并未添加用户表，所以可以先不管
	cout << "root root ";
	cout << inode.d_size << " ";
	cout << name;
	cout << endl;
return 0;
}

int FileSystem::show_file(int no)
{
	DiskInode inode;
	char buffer[512];
	disk->seekg(202 * BLOCK_SIZE + no * INODE_SIZE, ios::beg);
	disk->read((char*)(&inode), sizeof(inode));
	for (int i = 0; i < 10; i++)
	{
		//cout << inode.d_addr[i] << endl;//1091
		if (inode.d_addr[i] <= 0)
			break;
		disk->seekg(inode.d_addr[i] * BLOCK_SIZE, ios::beg);
		disk->read((char*)(buffer), sizeof(buffer));
		cout << buffer;
	}
	cout << endl;
	return 0;

}

int FileSystem::readInode(DiskInode & inode, fstream * disk, int no)
{
	;
	return 0;
}

FileSystem::FileSystem()
{
	disk = new fstream(DISK, ios::in | ios::out | ios::binary);
	if (!disk->is_open())
	{
		cout << "打开磁盘disk.img失败" << endl;
	}
	load_sb();//装载SuperBlock
	load_root();
	//memcpy(cur_node, root_node);
	cur_node = root_node;
	//cout << cur_node.d_size << endl;
	;
	file_list[0] = 0;
	file_list[1] = 1;
	file_list[2] = 2;
	file_point[0] = 0;
	file_point[1] = 0;
	file_point[2] = 0;
	for (int i = 3; i < 1024; i++)
	{
		file_list[i] = -1;
		file_point[i] = 0;
	}
}

FileSystem::~FileSystem()
{
	disk->close();
	delete disk;
}

int FileSystem::fls()
{
	int no;
	char name[28];
	for (int i = 0; i < 10; i++)
	{
		//cout <<  << endl;
		if (cur_node.d_addr[i] <= 0)//为0表示没有用到
			break;

		for (int j = 0; j < 16; j++)//每块可以16个目录项
		{
			disk->seekg(cur_node.d_addr[i] * BLOCK_SIZE + 32 * j, ios::beg);
			disk->read((char*)(&no), sizeof(no));
			disk->read((char*)(name), sizeof(name));
			if (no <= 0)
				break;
			//cout << no << " " << name << endl;
			string sname = name;
			show_ls(no, sname);//显示每一个DiskInode，也即每一个文件信息
			//no = 0; name[1] = '\0';

		}
	}
	return 0;
}

int FileSystem::fmkdir(string dir_name)
{
	int inode_num = IAlloc();
	//cout << "inode_num" << inode_num << endl;
	int block_num = Alloc();
	//cout << "block_num" << block_num << endl;
	//在当前Inode中注册新目录项
	int ii, jj;//ii*BLOCK_SIZE+0是当前的inode
	for (int i = 10 - 1; i >= 0; i--)
	{
		if (cur_node.d_addr[i] > 0)//最后1项
		{
			ii = i;
			break;
		}

	}
	for (int j = 2; j < 16; j++)
	{
		disk->seekg(cur_node.d_addr[ii] * BLOCK_SIZE + 32 * j, ios::beg);
		int m_ino;
		disk->read((char*)(&m_ino), sizeof(m_ino));
		//cout << m_ino << endl;
		if (m_ino == 0)//新目录项
		{
			jj = j;
			break;
		}
	}
	//cout << ii << " " << jj << endl;
	//cout << cur_node.d_addr[ii] << endl;
	disk->seekg(cur_node.d_addr[ii] * BLOCK_SIZE + 32 * jj, ios::beg);
	disk->write((char*)(&inode_num), sizeof(inode_num));
	disk->write((char*)(dir_name.c_str()), 28 * sizeof(char));


	//初始化新Inode
	DiskInode new_node;
	new_node.d_mode = 0xCDB6;
	new_node.d_nlink = 1;
	new_node.d_uid = 0;
	new_node.d_gid = 0;
	new_node.d_size = 0;
	new_node.d_addr[0] = block_num;
	for (int i = 1; i < 10; i++)
	{
		new_node.d_addr[i] = 0;
	}
	new_node.d_atime = 0;
	new_node.d_mtime = 0;
	disk->seekg(202 * BLOCK_SIZE + inode_num * INODE_SIZE, ios::beg);
	disk->write((char*)(&new_node.d_mode),sizeof(new_node.d_mode));
	disk->write((char*)(&new_node.d_nlink), sizeof(new_node.d_nlink));
	disk->write((char*)(&new_node.d_uid), sizeof(new_node.d_uid));
	disk->write((char*)(&new_node.d_gid), sizeof(new_node.d_gid));
	disk->write((char*)(&new_node.d_size), sizeof(new_node.d_size));
	disk->write((char*)(new_node.d_addr), sizeof(new_node.d_addr));
	disk->write((char*)(&new_node.d_atime), sizeof(new_node.d_atime));
	disk->write((char*)(&new_node.d_mtime), sizeof(new_node.d_mtime));
	
	//初始化新目录块
	//disk->seekg(block_num * BLOCK_SIZE, ios::beg);
	int cur_num=inode_num;
	int father_num;
	disk->seekg(cur_node.d_addr[ii] * BLOCK_SIZE + 32 * 0, ios::beg);
	disk->read((char*)(&father_num), sizeof(father_num));
	char cur_name[28] = ".";
	char father_name[28] = "..";
	disk->seekg(block_num * BLOCK_SIZE, ios::beg);
	disk->write((char*)(&cur_num), sizeof(cur_num));
	disk->write((char*)(cur_name), sizeof(cur_name));
	disk->write((char*)(&father_num), sizeof(father_num));
	disk->write((char*)(father_name), sizeof(father_name));
	return 0;
}

int FileSystem::fcd(string dir_name)
{
	DiskInode new_node;//要进入的文件夹
	char find_name[28];
	int find_num;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			disk->seekg(cur_node.d_addr[i] * BLOCK_SIZE + 32 * j, ios::beg);
			disk->read((char*)(&find_num), sizeof(find_num));
			disk->read((char*)(find_name), sizeof(find_name));
			if (dir_name == find_name)
			{
				disk->seekg(202 * BLOCK_SIZE + find_num * INODE_SIZE, ios::beg);
				disk->read((char*)(&new_node.d_mode),sizeof(new_node.d_mode));
				disk->read((char*)(&new_node.d_nlink), sizeof(new_node.d_nlink));
				disk->read((char*)(&new_node.d_uid), sizeof(new_node.d_uid));
				disk->read((char*)(&new_node.d_gid), sizeof(new_node.d_gid));
				disk->read((char*)(&new_node.d_size), sizeof(new_node.d_size));
				disk->read((char*)(new_node.d_addr), sizeof(new_node.d_addr));
				disk->read((char*)(&new_node.d_atime), sizeof(new_node.d_atime));
				disk->read((char*)(&new_node.d_mtime), sizeof(new_node.d_mtime));
				if (new_node.d_mode & 0x4000)
				{
					cur_node = new_node;
					return 0;
				}			
			}
		}
	}
	cout << "未找到文件夹" << endl;
	return -1;
}

int FileSystem::fcreate(string filename, int mode)
{
	int inode_num = IAlloc();
	//cout << "inode_num" << inode_num << endl;
	int block_num = Alloc();
	//cout << "block_num" << block_num << endl;
	DiskInode cur_file;
	cur_file.d_mode = 0x8C00;
	//cout << cur_file.d_mode << endl;
	cur_file.d_mode = cur_file.d_mode + (unsigned int)((mode/100)<<6);
	cur_file.d_mode = cur_file.d_mode + (unsigned int)((mode-(mode / 100)*100)/10 << 3);
	cur_file.d_mode = cur_file.d_mode + (unsigned int)((mode % 10));
	//cout << cur_file.d_mode << endl;
	cur_file.d_nlink = 0;
	cur_file.d_uid = 0;
	cur_file.d_gid = 0;
	cur_file.d_size = 0;
	cur_file.d_addr[0] = block_num;
	for (int i = 1; i < 10; i++)
	{
		cur_file.d_addr[i] = 0;
	}
	cur_file.d_atime = 0;
	cur_file.d_mtime = 0;
	writeInode(cur_file,disk,inode_num);

	int ii, jj;//ii*BLOCK_SIZE+0是当前的inode
	for (int i = 10 - 1; i >= 0; i--)
	{
		if (cur_node.d_addr[i] > 0)//最后1项
		{
			ii = i;
			break;
		}

	}
	for (int j = 2; j < 16; j++)
	{
		disk->seekg(cur_node.d_addr[ii] * BLOCK_SIZE + 32 * j, ios::beg);
		int m_ino;
		disk->read((char*)(&m_ino), sizeof(m_ino));
		//cout << m_ino << endl;
		if (m_ino == 0)//新目录项
		{
			jj = j;
			break;
		}
	}
	disk->seekg(cur_node.d_addr[ii] * BLOCK_SIZE + 32 * jj, ios::beg);
	disk->write((char*)(&inode_num), sizeof(inode_num));
	disk->write((char*)(filename.c_str()), 28 * sizeof(char));
	//在当前目录中注册文件
	return inode_num;
}

int FileSystem::fopen(string filename)
{
	int addr;
	int no;
	char name[28];
	for (int i = 0; i < 10; i++)
	{
		addr = cur_node.d_addr[i];
		disk->seekg(addr*BLOCK_SIZE,ios::beg);
		{
			for (int j = 0; j < 16; j++)
			{
				disk->read((char*)(&no),sizeof(no));
				disk->read((char*)(&name), sizeof(name));
				if (name == filename)
				{
					//cout << no << name << endl;
					for (int i = 3; i < 1024; i++)
					{
						if (file_list[i] < 0)
						{
							file_list[i] = no;
							cout << "打开文件成功,文件句柄" << i << endl;
							return i;
						}
					}
					cout << "文件句柄不够啦~,反正测试不到这里" << endl;
					
				}
			}
		}
	}
	cout << "打开文件失败" << endl;
	return 0;
}

int FileSystem::fdelete(string filename)
{
	int addr;
	int no;
	char name[28];
	string sname;
	for (int i = 0; i < 10; i++)
	{
		{
			for (int j = 0; j < 16; j++)
			{
				disk->seekg(cur_node.d_addr[i] * BLOCK_SIZE+j*32, ios::beg);
				disk->read((char*)(&no), sizeof(no));
				disk->read((char*)(name), sizeof(name));
				sname=name;
				//cout << no << name << endl;
				if (sname == filename)
				{
					//BFree(no);

					//释放Inode
					IFree(no);

					//目录注销
					int new_no=0;
					char new_name[28]="";
					disk->seekp(cur_node.d_addr[i] * BLOCK_SIZE + j * 32, ios::beg);
					disk->write((char*)(&new_no), sizeof(new_no));
					disk->write((char*)(new_name), sizeof(new_name));
					cout << "删除文件成功" << endl;
					return no;
				}
			}
		}
	}
	cout << "删除文件失败" << endl;
	return 0;
}

int FileSystem::fcat(string filename)
{
	int addr;
	int no;
	char name[28];
	int ii, jj;
	for (int i = 0; i < 10; i++)
	{
		addr = cur_node.d_addr[i];
		disk->seekg(addr*BLOCK_SIZE, ios::beg);
		{
			for (int j = 0; j < 16; j++)
			{
				disk->read((char*)(&no), sizeof(no));
				disk->read((char*)(&name), sizeof(name));
				if (name == filename)
				{
					//cout << no << endl;
					show_file(no);
					return 0;
				}
			}
		}
	}
	cout << "打开文件失败" << endl;
	return 0;
}

int FileSystem::fclose(int fd)
{
	file_list[fd] = -1;
	cout << "文件关闭" << endl;
	return 0;
}

int FileSystem::fwrite(int fd, string buffer, int length)
{
	int inode_no = file_list[fd];//找到该文件
	int pos = file_point[fd];
	DiskInode inode;
	disk->seekg(202 * BLOCK_SIZE + inode_no * INODE_SIZE,ios::beg);
	disk->read((char*)(&inode), sizeof(inode));
	int ii = pos / BLOCK_SIZE;
	pos = pos - ii * BLOCK_SIZE;
	
	disk->seekp(inode.d_addr[ii] * BLOCK_SIZE + pos, ios::beg);
	//cout << buffer.c_str() << endl;
	disk->write((char*)(buffer.c_str()), length);
	return 0;
}

int FileSystem::flseek(int fd, int position)
{
	file_point[fd] = position;
	return 0;
}

char * FileSystem::get_path()
{
	
	return nullptr;
}

int FileSystem::fread(int fd, string &sbuffer, int length)
{
	int no = file_list[fd];
	DiskInode inode;
	char buffer[512];
	disk->seekg(202 * BLOCK_SIZE + no * INODE_SIZE, ios::beg);
	disk->read((char*)(&inode), sizeof(inode));
	disk->seekg(inode.d_addr[0] * BLOCK_SIZE + file_point[fd], ios::beg);
	disk->read((char*)(buffer), length);
	buffer[length] = '\0';
	//cout << buffer << endl;
	sbuffer = buffer;
	//for (int i = 0; i < 10; i++)
	//{
	//	//cout << inode.d_addr[i] << endl;//1091
	//	if (inode.d_addr[i] <= 0)
	//		break;
	//	disk->seekg(inode.d_addr[i] * BLOCK_SIZE, ios::beg);
	//	disk->read((char*)(buffer), length);
	//	buffer[2] = '\0';
	//	cout << buffer << endl;
	//	sbuffer = buffer;
	//	
	//}
	cout << endl;
	return 0;

}
