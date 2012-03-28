obj-m += avkbd.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	cp avkbd.ko /lib/modules/`uname -r`/
	depmod -a

gentoo-install: all
	cp avkbd.ko /lib/modules/`uname -r`/
	depmod -a

ubuntu-install: all
	cp avkbd.ko /lib/modules/`uname -r`/
	depmod -a
	echo "avkbd" >> /etc/modules

