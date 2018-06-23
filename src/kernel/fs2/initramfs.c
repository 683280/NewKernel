
#include <fs/vfs.h>
#include <fs/initramfs.h>
#include <mm/string.h>
#include <mm/alloc.h>
#include <video/chr.h>

static inode_t *cpiofs_load(inode_t *inode)
{
	inode_t *rootfs = malloc(sizeof(inode_t));
	
	rootfs->name = NULL;
	rootfs->type = FS_DIR;
	rootfs->parent = NULL;
	rootfs->fs = &initramfs;
	rootfs->list = malloc(sizeof(dentry_t));
	dentry_t *list = rootfs->list;

	list->count = 0;
	
	dentry_t *tmp;
	
	cpio_hdr_t *cpio = ((ramdev_private_t*)inode->p)->ptr;
	
	printf("Loading ramdisk ");
	while(*(uint8_t*)cpio)
	{
		uint32_t size = cpio->filesize[0] * 0x10000 + cpio->filesize[1];
		uint8_t *name = (uint8_t*)cpio + sizeof(*cpio);
		if(!strcmp(name, "TRAILER!!!")) break;
		if(!strcmp(name, ".")) goto next;
		uint8_t *filename; uint32_t i;
		for( i = cpio->namesize - 1; i && (name[i] != '/'); --i);
		filename = name + (i?++i:i);
		uint8_t *path = strdup(name);
		path[i] = '\0';
		inode_type type = ((cpio->mode & 0170000 ) == 0040000)?FS_DIR:FS_FILE;

		void *data = name + cpio->namesize + cpio->namesize%2;
		inode_t *new_node = malloc(sizeof(inode_t));
		*new_node = 
			(inode_t)
			{
				.name = strdup(filename),
				.size = size,
				.type = type,
				.fs = &initramfs,
				.p = data,
			};
			
		vfs_create(rootfs, *path ? path : (uint8_t*)"/", new_node);
		free(path);
		
		next:
		cpio = (name + cpio->namesize + (cpio->namesize%2) + size + (size%2));
		printf(".");
	}
	printf(" [done]\n");

	return rootfs;
}

static uint64_t cpiofs_read(inode_t *inode, uint64_t offset, uint64_t len, void *buf_p)
{
	uint8_t *buf = (uint8_t*)buf_p;
	if(offset > inode->size) return 0;
	uint32_t size = MIN(len, inode->size - offset), _size = size;
	uint8_t *_buf = (uint8_t*)inode->p + offset;
	while(size--)
		*buf++ = *_buf++;

	return _size;
}

fs_t initramfs = {
		.name = "initramfs",
		.load = cpiofs_load,
		.read = cpiofs_read,
		.write = NULL,
	};
