/*
 * Simple Block Device Driver
 * reference : Linux Device Drivers 3rd edition
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/delay.h>

#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

static int major_num = 0;
module_param(major_num, int, 0);
static int logical_block_size = 512;
module_param(logical_block_size, int, 0);
static int nsectors = 2*1024*1024;	/* 1GB */
module_param(nsectors, int, 0);

#define KERNEL_SECTOR_SIZE 512
#define VM_NODE 1		/* total size : VM_NODE * 1GB */

static struct request_queue *Queue;

static struct ramdisk_device {
	unsigned long size;
	spinlock_t lock;
	u8 *data[VM_NODE];
	struct gendisk *gd;
}Device;

static void ramdisk_transfer(struct ramdisk_device *dev, sector_t sector,
		unsigned long nsect, char *buffer, int write)
{
	unsigned long nsectors_size = nsectors*512;
	unsigned int idx=0, nidx=0;
	unsigned long offset = sector * logical_block_size;
	unsigned long nbytes = nsect * logical_block_size, nnbytes = 0;

	if ((offset + nbytes) > dev->size)	return;

	idx = offset / nsectors_size;
	offset = offset % nsectors_size;

	if ((offset + nbytes) > nsectors_size)
	{
		nidx = idx+1;
		if(nidx >= VM_NODE)		return;
		nnbytes = (offset + nbytes) - nsectors_size;
		nbytes = nbytes - nnbytes;
	}

	if(write)
    {
//		printk(KERN_ERR "WRITE ramdisk\n");
        memcpy(dev->data[idx] + offset, buffer, nbytes);
        if(nidx){
            memcpy(dev->data[nidx], buffer+nbytes, nnbytes);
        }
    }
	else
	{
//		printk(KERN_ERR "READ ramdisk\n");
		memcpy(buffer, dev->data[idx] + offset, nbytes);
		if(nidx){
			memcpy(buffer+nbytes, dev->data[nidx], nnbytes);
		}
	}
}

static void ramdisk_request(struct request_queue *q)
{
	struct request *req;

	req = blk_fetch_request(q);
    while(req != NULL)
    {
#if 0
        if(req == NULL || (req->cmd_type != REQ_TYPE_FS))
        {
            printk(KERN_ERR "Skip non-CMD request \n");
            __blk_end_request_all(req, -EIO);
            continue;
        }
#endif

        ramdisk_transfer(&Device, blk_rq_pos(req), blk_rq_cur_sectors(req),
                bio_data(req->bio), rq_data_dir(req));
        if(!__blk_end_request_cur(req, 0))
        {
            req = blk_fetch_request(q);
        }
    }
}

int ramdisk_getgeo(struct block_device * block_device, struct hd_geometry * geo)
{
	long size;

	size = Device.size * (logical_block_size / KERNEL_SECTOR_SIZE);
	geo->cylinders = (size & ~0x3f) >> 6;
	geo->heads = 4;
	geo->sectors = 16;
	geo->start = 0;
	return 0;
}

static int ramdisk_ioctl(struct block_device *bdev, fmode_t mode, unsigned int cmd, unsigned long arg)
{
	int err = 0;

	switch(cmd){
        default:
			return -EFAULT;
			break;
	};
	return err;
}

static struct block_device_operations ramdisk_ops = {
	.owner = THIS_MODULE,
	.getgeo = ramdisk_getgeo,
	.ioctl = ramdisk_ioctl,
};

static int __init ramdisk_init(void)
{
	int i = 0;
	unsigned long size = nsectors * logical_block_size;
	Device.size = size * VM_NODE;

	printk(KERN_ERR "DEBUG_RAMDISK) Device total sectors: %d, total size: %ld MB\n", nsectors*VM_NODE, Device.size/1024/1024);

	spin_lock_init(&Device.lock);
	for(i = 0; i < VM_NODE; i++) {
		Device.data[i] = vmalloc(size);
		if(Device.data[i] == NULL) goto out;
	}

	Queue = blk_init_queue(ramdisk_request, &Device.lock);
	if(Queue == NULL) goto out;

	blk_queue_logical_block_size(Queue, logical_block_size);

	major_num = register_blkdev(major_num, "ramdisk");
	if(major_num < 0) {
		printk(KERN_ERR "ramdisk: unable to get major number\n");
		goto out;
	}

	Device.gd = alloc_disk(16);
	if(!Device.gd)
		goto out_unregister;
	Device.gd->major = major_num;
	Device.gd->first_minor = 0;
	Device.gd->fops = &ramdisk_ops;
	Device.gd->private_data = &Device;
	strcpy(Device.gd->disk_name, "ramdisk");
	set_capacity(Device.gd, nsectors * VM_NODE);
	Device.gd->queue = Queue;
	add_disk(Device.gd);

	printk(KERN_ERR "DEBUG_RAMDISK) Init Success\n");
	return 0;

out_unregister:
	unregister_blkdev(major_num, "ramdisk");

out:
    for(i = 0; i < VM_NODE; i++)
        if(Device.data[i])
            vfree(Device.data[i]);

    return -ENOMEM;
}

static void __exit ramdisk_exit(void)
{
    int i = 0;

    del_gendisk(Device.gd);
    put_disk(Device.gd);
    unregister_blkdev(major_num, "ramdisk");
    blk_cleanup_queue(Queue);

    for(i = 0; i < VM_NODE; i++)
        if(Device.data[i])
            vfree(Device.data[i]);
    printk(KERN_ERR "DEBUG_RAMDISK) Exit Success\n");
}

module_init(ramdisk_init);
module_exit(ramdisk_exit);

