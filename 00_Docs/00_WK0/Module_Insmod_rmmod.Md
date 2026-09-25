# Linux Kernel Internal Operations: insmod & rmmod

## 1. insmod (Module Insertion)
Dynamically loads a compiled kernel object (`.ko`) file into the running kernel.

### A. User Space Phase
* **Execution:** User runs `sudo insmod path/to/module.ko`.
* **File Parsing:** `insmod` opens the binary, reads its layout, and loads the raw data into user-space memory.
* **System Call:** Invokes `finit_module()` (takes a file descriptor) or `init_module()` (takes a memory buffer pointer).

### B. Kernel Space Phase (`sys_init_module`)
1. **Privilege & Sanity Verification**
   * Verifies the calling process has the `CAP_SYS_MODULE` capability.
   * `load_module()` inspects the ELF headers to ensure the architecture matches the running kernel.
2. **Memory Allocation**
   * Calculates the exact memory required for text, data, and BSS segments.
   * Allocates space inside kernel memory space using `vmalloc()`.
3. **Data Transfer**
   * Copies the module's code and data sections from user-space memory into the allocated kernel space.
4. **Relocation & Symbol Resolution**
   * Links unresolved external symbols (e.g., `printk`, `kmalloc`) against the kernel's internal symbol table (`/proc/kallsyms`).
   * Fails execution if required dependencies or symbols are missing.
5. **System Management Update**
   * Appends the module to the kernel's internal circular linked list of loaded modules.
   * Creates corresponding tracking entries in `/sys/module/` and `/proc/modules` (visible via `lsmod`).
6. **Execution (`module_init`)**
   * Jumps execution to the entry point designated by the driver's `module_init()` macro.
   * If the init function returns `0`, the module state updates to `MODULE_STATE_LIVE`.

---

## 2. rmmod (Module Removal)
Removes a running module from kernel memory using only its structural name.

### A. User Space Phase
* **Execution:** User runs `sudo rmmod module_name`.
* **System Call:** Invokes the `delete_module()` system call, passing the module name string.

### B. Kernel Space Phase (`sys_delete_module`)
1. **Lookup & Reference Count Validation**
   * Kernel walks the internal module linked list to locate the name.
   * Checks the module's reference counter (`refcnt`).
   * **Abort Condition:** If `refcnt > 0` (e.g., another module relies on it, or a user process holds an active file descriptor), the call aborts with *Device or resource busy*.
2. **State Transition**
   * Updates module state from `MODULE_STATE_LIVE` to `MODULE_STATE_GOING`.
   * This isolates the module, blocking any new references or access attempts.
3. **Execution (`module_exit`)**
   * Calls the cleanup routine registered via the driver's `module_exit()` macro.
   * **Cleanup Tasks Execution:** 
     * Unregisters character/block devices and sysfs entries.
     * Releases requested hardware resources (Interrupt requests/IRQs, I/O memory regions).
     * Frees kernel buffers (`kfree`).
4. **Resource Reclamation**
   * Removes the module node completely from the internal linked list.
   * Tears down files under `/sys/module/`.
   * Frees the backing virtual memory pages allocated during the insertion phase.

---

## 3. Comparative Summary

| Phase | insmod Operation | rmmod Operation |
| :--- | :--- | :--- |
| **Input Target** | Path to the binary file (`.ko`) | String name of the loaded module |
| **Triggered Syscall** | `finit_module` / `init_module` | `delete_module` |
| **Primary Kernel Call** | `load_module()` | `sys_delete_module()` |
| **Driver Macro Invoked**| `module_init()` | `module_exit()` |
| **Typical Failure Mode**| Unresolved external symbols | Active reference count (`refcnt` > 0) |
