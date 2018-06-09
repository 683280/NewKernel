#include <vfs.h>

#include <initramfs.h>
#include <devfs.h>
#include <ext2.h>
#include <string.h>
#include <chr.h>

fs_t * filesystems[] = 
{
	&initramfs,
	&devfs,
	&ext2fs,
	NULL,
};

static void fsman_init()
{
	printf("Loading filesystems ");
	fs_t **fs = filesystems;
	while(*fs)
	{
		printf(".");
		++fs;
	}

	printf(" [done]\n");
}

static fs_t * fsman_getfs(uint8_t *name)
{
	if(!name) return NULL;
	fs_t **fs = filesystems;
	while(*fs)
	{
		if((*fs)->name && !strcmp(name, (*fs)->name))
			return *fs;
		++fs;
	}
	return NULL;
}

fsman_t fsman = {
	.init 	= &fsman_init,
	.getfs	= &fsman_getfs, 
};
