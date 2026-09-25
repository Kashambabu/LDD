#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xc0107976, "proc_create" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x37a0cba, "kfree" },
	{ 0x122c3a7e, "_printk" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x679a9726, "proc_remove" },
	{ 0x778e2fc6, "__kmalloc_cache_noprof" },
	{ 0x190e6fd0, "kmalloc_caches" },
	{ 0x7cf6715, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9EAB38DB53C31707C63550F");
