obj-m := example.o
KDIR := /lib/modules/$(shell uname -r)/build
M := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
