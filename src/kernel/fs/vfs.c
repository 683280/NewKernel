
#include <fs/vfs.h>
#include <mm/string.h>
#include <mm/alloc.h>
#include <video/chr.h>
#include <mm/mm.h>

inode_t *vfs_root;

void vfs_mount_root(inode_t *node)
{
	vfs_root = node;
	vfs_root->fs->init(vfs_root);
}
inode_t* find_inode_by_path(inode_t* inode,uint8_t* path){
    if(!inode || !inode->list || !inode->list->count)return NULL;
    inode_t* tmp = inode->list->head;
    uint32_t i = 0;
    while (tmp && i < inode->list->count) {
        if(strcmp(tmp->name,path))return tmp;
        tmp = tmp->next;
        i++;
    }
    return NULL;
}

inode_t *vfs_trace_path(inode_t *inode, uint8_t *_path) {
//	dprintf("<<<<<<<<<<vfs_trace_path>>>>>>>>>>\n");

	if(!_path || (_path && !*_path)) return inode;

	uint8_t *path = strdup(_path);
	uint32_t len = strlen(path);
	uint8_t *end_path = path + len;
//	dprintf("path = %s   len = %d \n",path,strlen(_path));
	uint32_t dir_levels = 1;
	uint32_t i;
	for( i = 0; i < len; ++i)
		if(path[i] == '/') {
            path[i] = '\0';
            dir_levels++;
        }


	if((!*path) && len == 1) return inode;
	if(!*path) ++path;

	inode_t *tmp = inode;
//    dprintf("inode = 0x%08x   count = %d\n",inode,inode->list->count);
//
//    dprintf("while(tmp) \n");
	while(dir_levels--){

        tmp = find_inode_by_path(tmp,path);
        if(!tmp)return NULL;

        path += strlen(path);
	}
    return tmp;
}

inode_t *vfs_create(inode_t *root, uint8_t *path, inode_t *new_node)
{
	inode_t *dir = vfs_trace_path(root, path);
	
	if(!dir) return NULL;
	if(!dir->list)
	{
		dir->list = malloc(sizeof(dentry_t));
		dir->list->count = 0;
	}
	uint32_t k;
	
	if(!dir->list->head) dir->list->head = malloc(sizeof(inode_t));
	
	++dir->list->count;
	new_node->next = dir->list->head;
	new_node->parent = dir;
	dir->list->head = new_node;
	return new_node;
}

inode_t *vfs_mount(inode_t *root, uint8_t *path, inode_t *new_node)
{
	inode_t *dir = vfs_trace_path(root, path);
	if(!dir) return NULL;
	
	inode_t *tmp = malloc(sizeof(inode_t));
	memcpy(tmp, dir, sizeof(inode_t));
	
	dir->type = FS_MOUNTPOINT;

	vfs_mountpoint_t *mount = malloc(sizeof(vfs_mountpoint_t));
	mount->inode = new_node;
	mount->old_inode = tmp;
	
	dir->p    = mount;
	//dir->fs   = &vfs;
	
	return dir;
}

void vfs_tree(inode_t *node)
{
	static uint32_t level = 0;
	if(!node) return;
	if(node->name) printf("%s", node->name);
	if(node->type == FS_DIR || node->type == FS_MOUNTPOINT) 
	{
		printf("/");
		if(node->type == FS_MOUNTPOINT)
		{
			vfs_mountpoint_t *m = node->p;
			vfs_tree(m->inode);
			return;
		}
		if(node->list)
		{
			printf("\n");
			++level;
			uint32_t i, j;
			for(j = 0; j < node->list->count; ++j)
			{
				if(level > 1)
					for(i = 1; i < 2 * level - 2; ++i) printf(" ");
				printf("→ ");
				
				inode_t *tmp = node->list->head;
				uint32_t k = 0;
				while(k++ < j) tmp = tmp->next;
				vfs_tree(tmp);
				printf("\n");
			}
			--level;
			return;
		}
	}
}

uint64_t vfs_read(inode_t *inode, uint64_t offset, uint64_t len, void *buf)
{
	return inode->fs->read(inode, offset, len, buf);
}

uint64_t vfs_write(inode_t *inode, uint64_t offset, uint64_t len, void *buf)
{
	return inode->fs->write(inode, offset, len, buf);
}

inode_t* vfs_open(char* filename){
//    dprintf("vfs_root = 0x%08x    count = %d\n",vfs_root,vfs_root->list->count);
    inode_t *dir = vfs_trace_path(vfs_root, filename);
    return dir;
}