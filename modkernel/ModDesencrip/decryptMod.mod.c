#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x4db908d1, "module_layout" },
	{ 0xfe3a1ba6, "class_unregister" },
	{ 0x7da323b0, "device_destroy" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x51c8a752, "device_create" },
	{ 0xc928fab, "class_destroy" },
	{ 0xe5f3b058, "__class_create" },
	{ 0xaef55450, "__register_chrdev" },
	{ 0xad27f361, "__warn_printk" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa916b694, "strnlen" },
	{ 0xe914e41e, "strcpy" },
	{ 0x7c32d0f0, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1EEC625D72EA4CD81232F71");
