obj-m := dk_main.o
KDIR = ../openwrt/build_dir/target-arm_cortex-a7+neon-vfpv4_musl-1.1.11_eabi/linux-brcm2708_bcm2709/linux-4.1.11

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KDIR) M=$(shell pwd) modules

clean:
	    make -C $(KDIR) M=$(shell pwd)clean
