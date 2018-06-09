#ifndef _VFS_H
#define _VFS_H

#include <type.h>

typedef struct filesystem_struct fs_t;
typedef enum { FS_FILE, FS_DIR, FS_CHRDEV, FS_BLKDEV, FS_SYMLINK, FS_PIPE, FS_MOUNTPOINT } inode_type; 
typedef struct inode_struct inode_t;
typedef struct dentry_struct dentry_t;

typedef struct filesystem_struct {
	//文件系统名字
	uint8_t		*name;
	//加载文件系统
	inode_t*	(*load)  (inode_t*);
	//
	inode_t*	(*load_inode) (inode_t*);
	uint32_t	(*link)  (inode_t*, uint8_t*);
	uint32_t	(*unlink)(inode_t*, uint8_t*);
	//
	uint32_t 	(*create_inode) (inode_t*);
	//读取文件系统
	uint64_t	(*read)  (inode_t*, uint64_t, uint64_t, void*);
	//写出文件系统
	uint64_t	(*write) (inode_t*, uint64_t, uint64_t, void*);
	//
	uint32_t 	(*cheack) (inode_t*);
	//
	uint32_t	(*ioctl) (inode_t*, uint64_t, ...);
	//挂载文件系统
	uint32_t	(*mount) (inode_t*, inode_t*);
}fs_t;

#include <device.h>

struct inode_struct {
	//文件名
	uint8_t		*name;
	//文件大小
	uint32_t	size;
	//文件类型
	inode_type	type;
	//
	uint32_t 	c_time;
	uint32_t 	e_time;
	uint32_t 	l_time;
	//父文件
	inode_t 	*parent;
	//子文件
	dentry_t	*list;
	fs_t		*fs;	//文件系统
	dev_t		*dev;	//文件系统在哪个设备
	void		*p;	// To be used by filesystem handler//文件系统处理函数
	void		*d_data;

	inode_t		*next;	// For directories
};

struct dentry_struct
{
	uint32_t count;
	inode_t *head;
};

typedef struct vfs_mountpoint_struct
{
	inode_t *inode;
	inode_t *old_inode;
}vfs_mountpoint_t;

typedef struct dirent {
	uint32_t d_ino;
	char d_name[256];
} dirent;

typedef struct
{
	void   (*init) ();
	fs_t * (*getfs)(uint8_t *name);
} fsman_t;

extern fsman_t fsman;

extern fs_t vfs;

extern inode_t *vfs_root;
inode_t *vfs_trace_path(inode_t*, uint8_t*);
inode_t *vfs_create(inode_t*, uint8_t*, inode_t*);
inode_t *vfs_mount(inode_t*, uint8_t*, inode_t*);
void vfs_tree(inode_t*);

uint64_t vfs_read (inode_t*, uint64_t, uint64_t, void*);
uint64_t vfs_write(inode_t*, uint64_t, uint64_t, void*);

#endif
