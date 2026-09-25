# =========================================================================
# CONFIGURATION: Change these variables for different projects
# =========================================================================
MODULE_NAME    := HelloWorld
SRC_DIR        := src
INC_DIR        := inc


# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)

SRCS := $(wildcard $(src)/$(SRC_DIR)/*.c)
OBJS := $(patsubst $(src)/%, %, $(SRCS:.c=.o))

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-objs := $(OBJS)

# Additional include paths
ccflags-y += -I$(src)/$(INC_DIR)

$(info [DEBUG KBUILD] KERNELRELEASE is active: $(KERNELRELEASE))

# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else

# Kernel Directory Path
KDIR ?=/lib/modules/$(shell uname -r)/build

# The current directory is passed to sub-makes as argument
PWD := $(shell pwd)

all: 
	@echo "===================================================="
	@echo "              BUILD TARGET		                   "
	@echo "===================================================="
	make -C $(KDIR) M=$(PWD) modules

clean:
	@echo "===================================================="
	@echo "              CLEAN TARGET		                   "
	@echo "===================================================="
	make -C $(KDIR) M=$(PWD) clean

.PHONY: all clean

endif