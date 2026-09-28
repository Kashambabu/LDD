# Linux Kernel Module Compilation Steps

## 1. Check Linux Kernel Version

Before building a Linux Device Driver (LDD) or kernel module, identify the running kernel version.

```bash
cat /proc/version
```

To get only the running kernel version:

```bash
uname -r
```

Example:

```text
6.12.109+rpt-rpi-v8
```

> **Note:** `/proc/version` shows the compiler used to build the kernel. For an external module, the important requirement is to use the **matching kernel build/headers**.

---

## 2. Check Kernel Headers

Verify that the kernel build directory is available:

```bash
ls -ld /lib/modules/$(uname -r)/build
```

This directory provides the kernel build environment required to compile external modules.

---

## 3. Configure VS Code

Open the VS Code Command Palette:

```text
Ctrl + Shift + P
```

Select:

```text
C/C++: Edit Configurations (JSON)
```

Configure `.vscode/c_cpp_properties.json` similar to:

```json
{
    "configurations": [
        {
            "name": "Linux (Raspberry Pi)",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/include",
                "/usr/local/include",
                "/lib/modules/6.12.109+rpt-rpi-v8/build/include",
                "/lib/modules/6.12.109+rpt-rpi-v8/build/arch/arm64/include"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-gcc-arm64"
        }
    ],
    "version": 4
}
```

### Important Parameters

* **`includePath`** – Header file locations.
* **`compilerPath`** – Compiler used by VS Code IntelliSense.
* **`cStandard`** – C language standard.
* **`cppStandard`** – C++ standard; normally not required for kernel modules.
* **`intelliSenseMode`** – Target compiler/architecture mode.

> **Note:** `c_cpp_properties.json` is mainly for VS Code IntelliSense. Actual kernel-module compilation is performed using the Linux **Kbuild** system.

---

## 4. Makefile for Linux Device Driver Compilation

A Linux kernel module should normally be compiled using the kernel's **Kbuild** system.

The following Makefile command:

```make
make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
```

first enters the kernel build directory and then Kbuild processes the module source files, headers, and kernel configuration required for the build.

### 4.1 Single Source File

[Single source file Makefile Example](../../01_LddBasics/00_WK0/00_HelloWorld/Makefile)

For a simple driver containing only `HelloWorld.c`:

```make
obj-m += HelloWorld.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

Build:

```bash
make
```

Clean:

```bash
make clean
```

---

### 4.2 Multiple Source Files
[Multiple source files Makefile Example](../../01_LddBasics/00_WK0/02_Multiplefiles/Makefile)

If the driver consists of multiple source files, for example:

```text
HelloWorld.c
File1.c
File2.c
```

use:

```make
obj-m += HelloWorld.o

HelloWorld-objs := File1.o File2.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

Here, `File1.o` and `File2.o` are combined to create the final `HelloWorld.ko` kernel module.

---

### 4.3 Multiple Folders
[Multiple Folders Makefile Example](../../01_LddBasics/00_WK0/03_MultipleFolders/Makefile)

For a project with separate source and include directories:

```text
HelloWorld/
├── Makefile
├── src/
│   ├── File1.c
│   └── File2.c
└── inc/
    └── File2.h
```

Use:

```make
obj-m += HelloWorld.o

HelloWorld-objs := src/File1.o src/File2.o

OUT := $(PWD)/out

# Additional include path
ccflags-y += -I$(PWD)/inc

all: | $(OUT)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) MO=$(OUT) modules

$(OUT):
	mkdir -p $@

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

Here:

* `src/` contains the driver source files.
* `inc/` contains project-specific header files.
* `out/` contains the generated kernel-module build output.
* `ccflags-y` adds additional include directories.
* `MO=$(OUT)` specifies the output directory for the module build.

---

## 5. Standard/Re-usable Makefile
[Standard/Re-usable Makefile Example](../../01_LddBasics/00_WK0/04_StdMake/Makefile)

For a reusable project structure, the following Makefile can be used:

```make
# =========================================================================
# CONFIGURATION
# =========================================================================

MODULE_NAME := HelloWorld
SRC_DIR     := src
INC_DIR     := inc
OUT         := $(PWD)/out

# =========================================================================
# KERNEL BUILD
# =========================================================================

ifneq ($(KERNELRELEASE),)

# Source files
SRCS := $(wildcard $(src)/$(SRC_DIR)/*.c)

# Convert .c files to .o files
OBJS := $(patsubst $(src)/%,%,$(SRCS:.c=.o))

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-objs := $(OBJS)

# Additional include path
ccflags-y += -I$(src)/$(INC_DIR)

else

# Kernel build directory
KDIR ?= /lib/modules/$(shell uname -r)/build

# Current project directory
PWD := $(shell pwd)

all: | $(OUT)
	@echo "===================================================="
	@echo "              BUILD TARGET"
	@echo "===================================================="
	$(MAKE) -C $(KDIR) M=$(PWD) MO=$(OUT) modules

$(OUT):
	mkdir -p $@

clean:
	@echo "===================================================="
	@echo "              CLEAN TARGET"
	@echo "===================================================="
	$(MAKE) -C $(KDIR) M=$(PWD) clean

.PHONY: all clean

endif
```

### Build Commands

Build the driver:

```bash
make
```

Clean the generated files:

```bash
make clean
```

The advantage of this structure is that the **same Makefile can be reused for different driver projects** by changing:

```make
MODULE_NAME := HelloWorld
SRC_DIR     := src
INC_DIR     := inc
```

The kernel build system is invoked through `KDIR`, while the driver source files, include files, and module-specific build options are handled by Kbuild.
