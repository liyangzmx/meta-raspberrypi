DESCRIPTION = "Linux Kernel for Raspberry Pi"
# 包类别
SECTION = "kernel"
# 配方的源许可证列表
LICENSE = "GPLv2"
# 配方源代码中许可证文本的校验和
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

# 该配方只为rpi开头的machine兼容
COMPATIBLE_MACHINE = "^rpi$"

# 包的porch, 可理解为一种版本代号
PE = "1"
# 包版本
PV = "${LINUX_VERSION}+git${SRCPV}"

inherit siteinfo
require recipes-kernel/linux/linux-yocto.inc

# 包源码所指向的位置
SRC_URI += " \
    ${@bb.utils.contains("INITRAMFS_IMAGE_BUNDLE", "1", "file://initramfs-image-bundle.cfg", "", d)} \
    ${@bb.utils.contains("MACHINE_FEATURES", "vc4graphics", "file://vc4graphics.cfg", "", d)} \
    "

KCONFIG_MODE = "--alldefconfig"
# 指定内核中的配置文件, 形式: KBUILD_DEFCONFIG_{KMACHINE}, 意味着针对制定的machine分别配置
KBUILD_DEFCONFIG_raspberrypi0-wifi ?= "bcmrpi_defconfig"
KBUILD_DEFCONFIG_raspberrypi ?= "bcmrpi_defconfig"
KBUILD_DEFCONFIG_raspberrypi-cm3 ?= "bcm2709_defconfig"
KBUILD_DEFCONFIG_raspberrypi2 ?= "bcm2709_defconfig"
KBUILD_DEFCONFIG_raspberrypi3 ?= "bcm2709_defconfig"
KBUILD_DEFCONFIG_raspberrypi3-64 ?= "bcmrpi3_defconfig"
KBUILD_DEFCONFIG_raspberrypi4 ?= "bcm2711_defconfig"
KBUILD_DEFCONFIG_raspberrypi4-64 ?= "bcm2711_defconfig"

# linux的扩展版本号
LINUX_VERSION_EXTENSION ?= ""

# linux引导过程中自动加载的模块
KERNEL_MODULE_AUTOLOAD += "${@bb.utils.contains("MACHINE_FEATURES", "pitft28r", "stmpe-ts", "", d)}"

# 如果是uImage, 编译内核时, 额外制定一个宏定义
# A LOADADDR is needed when building a uImage format kernel. This value is not
# set by default in rpi-4.8.y and later branches so we need to provide it
# manually. This value unused if KERNEL_IMAGETYPE is not uImage.
KERNEL_EXTRA_ARGS += "LOADADDR=0x00008000"
