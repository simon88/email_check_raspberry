obj-m := email_check.o

ARCH ?= arm
KDIR := ~/Documents/projet/raspberry/kernel_build/linux
CROSS_COMPILE ?= /home/simon/pidev/pitools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-

PWD := $(shell pwd)
all:
	$(MAKE) -C $(KDIR) ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} M=$(PWD) modules
clean:
	rm -f  *.o  .*.o  .*.o.* *.ko  .*.ko  *.mod.* .*.mod.* .*.cmd
	rm -f Module.symvers Module.markers modules.order
	rm -rf .tmp_versions
