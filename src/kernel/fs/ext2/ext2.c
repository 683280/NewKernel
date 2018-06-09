#include <vfs.h>
#include <ext2.h>
#include <alloc.h>
#include <string.h>
#include <chr.h>
#include <mm.h>

#define ext2_inode_size sizeof(ext2_inode_t)

#define max_cache_bgd 5
typedef struct block_group_node{
    uint32_t block_group_id;
    block_group_descriptor_t* bgd;
    struct block_group_node* next;
}block_group_node;

uint32_t *get_block(inode_t *dev_inode, uint32_t block_id,uint32_t offset,uint32_t size, void *buf) {

    ext2_private_t*ext2_private = dev_inode->p;


    uint32_t bs = 1024 << ext2_private->sb->block_size;

    size = size ? size : bs;
//    printf("bs = %d\n",bs);
    dev_inode->dev->read(dev_inode->d_data,(block_id * bs) + offset, size, buf);
//    printf("get_block end \n");
    return buf;
}
block_group_node *bgn_cache = 0;
block_group_descriptor_t* get_block_group_desc(inode_t *dev_inode, ext2_superblock_t *sb,uint32_t block_group){

    uint32_t block_group_b;
    block_group_node* bgn = bgn_cache;
    int i;
    for (i = 0; i < max_cache_bgd && bgn; ++i) {
        if (bgn->block_group_id == block_group){
            //有缓存直接返回
            return bgn->bgd;
        }
        bgn = bgn->next;
    }
    printf("read block group %d\n",block_group);
    block_group_descriptor_t*  bgd = malloc(sizeof(block_group_descriptor_t));
    //没有缓存，重新读取块描述符
    block_group_b = (block_group * sb->blocks_per_block_group) + 1;
    //读取块组描述符
    get_block(dev_inode,block_group_b,0, sizeof(block_group_descriptor_t),bgd);

    //判断缓存数量是否大于最大值
    if (i >= max_cache_bgd){
        free(bgn->bgd);
    } else{
        bgn = malloc(sizeof(block_group_node));
    }
    //加入缓存
    bgn->block_group_id = block_group;
    bgn->bgd = bgd;
    bgn->next = bgn_cache;

    bgn_cache = bgn;

    return bgd;
}
ext2_inode_t *ext2_get_inode(inode_t *dev_inode, ext2_superblock_t *sb, uint32_t inode)
{
    uint32_t offset;
    //计算块大小
	uint32_t bs = 1024 << sb->block_size;
    //计算在哪个块组
	uint32_t block_group = (inode - 1) / sb->inodes_per_block_group;
	//给块组描述符 分配内存空间
	block_group_descriptor_t *bgd = get_block_group_desc(dev_inode,sb,block_group);
	//获取inode在当前块组的位置 0 开始
	uint32_t index = (inode - 1) % sb->inodes_per_block_group;
	uint32_t tem = bs / ext2_inode_size;
    uint32_t block_i = (index / tem) + bgd->inode_table;
	//给新的inode分配空间
	ext2_inode_t *i = malloc(ext2_inode_size);
	//读取这个inode
    offset = (index % tem) * ext2_inode_size;
//    printf("ext2_get_inode \n");
    get_block(dev_inode,block_i - 1,offset,ext2_inode_size,i);
//    printf("ext2_get_inode end \n");
//    printf("block i = %d  offset = %d size = %d\n",block_i - 1,offset,i->size);
    return i;
}


uint32_t *get_block_ptrs(inode_t *dev_inode, ext2_superblock_t *sb, ext2_inode_t *inode) {
    //块大小
	uint32_t bs = 1024 << sb->block_size;
    //占用多少个块
	uint32_t isize = inode->size / bs + (inode->size % bs ? 1 : 0 );
	//给占用的每个块分配一个数组 第一个存放isize
	uint32_t *blks = malloc(sizeof(uint32_t) * (isize + 1));
	// The size is always saved as the first entry of the returned array
	blks[0] = isize;

	uint32_t k;
	for(k = 1; k <= 12 && k <= isize; ++k)
		blks[k] = inode->direct_pointer[k - 1];

	if(isize <= 12)	// Was that enough !?
		return blks;

	isize -= 12;


	uint32_t *tmp = get_block(dev_inode, inode->singly_indirect_pointer,0,bs, malloc(bs));
    uint32_t num = bs / 4;
	for(k = 0; k < num && k <= isize; ++k)
		blks[13 + k] = tmp[k];

	free(tmp);

	if(isize <= num)	// Are we done yet !?
		return blks;

	isize -= num;

	tmp = get_block(dev_inode, inode->doubly_indirect_pointer,0,bs, malloc(bs));
	uint32_t *tmp2 = malloc(bs);

	uint32_t j;
	for(k = 0; k < num && (k * num) <= isize; ++k){

		tmp2 = get_block(dev_inode, tmp[k],0,bs, tmp2);
		for(j = 0; j < num && (k * num + j) <= isize; ++j)
		{
			blks[13 + num + k * num + j] = tmp2[j];
		}
	}

	if(isize <= num*num)
		return blks;

	free(tmp);

	// TODO : Support triply indirect pointers
	//debug("File size too big\n");
	for(;;);
}
void setup_inode(inode_t *dev_inode,inode_t *new_inode,ext2_inode_t*inoe,ext2_dentry_t *d){
    ext2_private_t *t_p ;

    new_inode->name = strndup(&d->name, d->name_length);
    new_inode->name = strndup(d->name, d->name_length);
    new_inode->size = inoe->size;
    new_inode->c_time = inoe->creation_time;
    new_inode->e_time = inoe->last_modified_time;
    new_inode->l_time = inoe->last_access_time;

    free(inoe);

    new_inode->type = inoe->type;
    new_inode->dev  = dev_inode->dev;
    new_inode->fs   = &ext2fs;

    t_p = malloc(sizeof(ext2_private_t));
// 	t_p->sb = sb;
    t_p->inode = d->inode;
	d->inode = t_p->inode;
    new_inode->p = t_p;
    if (t_p->inode != d->inode){
        printf("t_p->inode != d->inode  ");
    }
    printf("inode = %d filesize = %d create = %d p = %08x\n",t_p->inode, new_inode->size,new_inode->c_time,t_p);
}

inode_t *ext2_load(inode_t *dev_inode) {

	ext2_private_t*ext2_private = dev_inode->p;

    ext2_superblock_t *sb = malloc(sizeof(ext2_superblock_t));
    ext2_private->sb = sb;
    //读取super_block
    get_block(dev_inode,0,0, sizeof(ext2_superblock_t), sb);

	if(sb->ext2_signature != 0xEF53) return NULL;

	uint32_t bs = 1024 << sb->block_size;	// Block size
	uint32_t is = sb->inode_size;			// Inode size
	
    printf("load root_inode\n");

    ext2_inode_t * root_inode = ext2_get_inode(dev_inode,sb,2);
//    root_inode = ext2_get_inode(dev_inode,sb,13);
    printf("load root_inode end \n");

    uint32_t *ptrs = get_block_ptrs(dev_inode, sb, root_inode);
	uint32_t count = *ptrs++;
	uint32_t k;
	ext2_dentry_t *_d = malloc(bs), *d;

    inode_t *ext2_vfs = malloc(sizeof(inode_t));
	ext2_vfs->name = strdup("hd");
	ext2_vfs->type = FS_DIR;
	ext2_vfs->list = malloc(sizeof(dentry_t));
	ext2_vfs->list->count = 0;
	ext2_vfs->list->head = NULL;
	ext2_vfs->c_time = root_inode->creation_time;
	ext2_vfs->e_time = root_inode->last_modified_time;
	ext2_vfs->l_time = root_inode->last_access_time;
	inode_t *_tmp = NULL;
//	printf("i = %x   creation_time = %d \n",root_inode,root_inode->creation_time);
	for(k = 0; k < count; ++k){
		d = (get_block(dev_inode, *(ptrs + k) - 1,0, bs, _d));

        if(d){

			uint32_t size = 0;
			while(size <= bs){
                if((!d->size) || (size + d->size > bs)) break;
				if(!_tmp)
					ext2_vfs->list->head = _tmp = malloc(sizeof(inode_t));
				else
					_tmp = _tmp->next = malloc(sizeof(inode_t));

				_tmp->next = NULL;

				ext2_inode_t *_inode = ext2_get_inode(dev_inode, sb, d->inode);
                setup_inode(dev_inode,_tmp,_inode,d);

				ext2_vfs->list->count++;

				size += d->size;
				d = (ext2_dentry_t*)((uint64_t)d + d->size);
			}
			//free(d);
		}
	}

	free(_d);

	return ext2_vfs;
}

uint32_t ext2_read(inode_t *inode, uint32_t offset, uint64_t size, void *buf) {
	uint32_t _size = size > inode->size ? inode->size : size;
	//debug("Reading file %s\n", inode->name);
	ext2_private_t *p = inode->p;
    printf("p = %08x  size = %d inode = %d  %08x\n",p,size,p->inode,p->sb->ext2_signature);

	uint32_t bs = 1024 << p->sb->block_size;
	ext2_inode_t *i = ext2_get_inode(inode, p->sb, p->inode);
    printf("i = %d",i);
	uint32_t *ptrs = get_block_ptrs(inode, p->sb, i);
	uint32_t count = *ptrs++;
	uint32_t skip_blocks = offset / bs; // Blocks to be skipped
	uint32_t skip_bytes  = offset % bs; // Bytes  to be skipped

	if(count < skip_blocks) return 0;

	count -= skip_blocks;
	ptrs  += skip_blocks;

	// Now let's read the first part ( size <= bs )
	uint32_t first_part_size = (count > 1) ? MIN(bs - skip_bytes, size) : MIN(size, inode->size);
	inode->dev->read(inode, (*ptrs++) * bs + skip_bytes, first_part_size, buf);
	buf += first_part_size;
	size -= first_part_size;
	--count;

	// Now let's read the second part ( size is a multible of bs )
	while(count > 1 && size > bs)
	{
		get_block(inode, *ptrs++,0, bs, buf);
		buf += bs;
		size -= bs;
		--count;
	}

	// Now let's read the last part ( size <= bs)
	if(count){
		uint32_t last_part_size = MIN(size, inode->size - ( _size - size ));
		get_block(inode, (*ptrs++),0, last_part_size, buf);
		buf += last_part_size;
		size -= last_part_size;
		--count;
	}

	return _size - size;

	return 0;
}

static uint32_t ext2_mount(inode_t *dst, inode_t *src)
{
	inode_t *i = ext2_load(src);

	if(!i) return -1;
	vfs_mount(dst, "/", i);

	return 1;
}

/*
file_t *ext2_open(inode_t *inode)
{
	file_t *ret = malloc(sizeof(file_t));
	ret->pos = 0;
	ret->size = inode->size;
	ret->buf = malloc(30);
	//debug("size %d\n", inode->size);
	inode->fs->fs->read(inode, 0ret->buf, 20);
	return ret;
}
*/
fs_t ext2fs = {
		.name = "ext2",
		.load = ext2_load,
		//.open = NULL,
		.read = ext2_read,
		.write = NULL,
		.mount = &ext2_mount,
	};
