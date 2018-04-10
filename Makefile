
ifneq ($(KERNELRELEASE),)
	obj-m := ramdisk.o

else

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) V=$(V) modules
endif
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean

#clean:
#	rm -rf *.ko
#	rm -rf *.mod.*
#	rm -rf .*.cmd
#	rm -rf *.o
#	rm -rf .tmp_versions
