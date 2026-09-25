obj-m += HelloWorld.o
HelloWorld-objs := src/File1.o src/File2.o

OUT := $(PWD)/out
# Additional include paths
ccflags-y += -I$(PWD)/inc

all: | $(OUT)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) MO=$(OUT) modules

$(OUT):
	mkdir -p $@

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean