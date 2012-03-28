obj-m += avms.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	cp avms.ko /lib/modules/`uname -r`/
	depmod -a

ubuntu-install:
	echo "avms" >> /etc/modules

