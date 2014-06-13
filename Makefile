obj-m := alienfx.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load:
	sudo rmmod  alienfx
	sudo insmod alienfx.ko

help:
	$(MAKE) -C $(KDIR) M=$(PWD) help

.PHONY: install
install: all load
	sudo $(MAKE) -C $(KDIR) M=$(PWD) modules_install
	sudo depmod

