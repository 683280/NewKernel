#ifndef _DEVICE_H
#define _DEVICE_H

#include <va_list.h>

typedef enum
{
	DEV_CHR = 1,
	DEV_BLK = 2,
} dev_type;

typedef struct dev_struct dev_t;

#include <vfs.h>

struct dev_struct
{
	//设备名
	uint8_t		*name;
	//设备类型
	dev_type	type;
	//检查设备
	uint32_t	(*probe)(void *p);
	//读设备
	uint32_t	(*read) (void *p, uint32_t offset, uint32_t size, void *buf);
	//写设备
	uint32_t	(*write)(void *p, uint32_t offset, uint32_t size, void *buf);
	uint32_t	(*ioctl)(void *p, uint64_t request, va_list args);
//	void 		*p;	// To be used by device handler
} __attribute__((packed));

extern dev_t ramdev;
extern dev_t condev;

typedef struct ramdev_private_struct
{
	void *ptr;
	uint64_t size;
}ramdev_private_t;

typedef struct
{
	void (*init)(void);
} devman_t;

extern devman_t devman;


void init_devices();
#endif
