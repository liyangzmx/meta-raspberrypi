# linux内核版本
LINUX_VERSION ?= "5.10.13"
# 内核的分支设置
LINUX_RPI_BRANCH ?= "rpi-5.10.y"

SRCREV_machine = "34263dc81a12862c66e2593bb26c09d5fd20f46d"
SRCREV_meta = "5833ca701711d487c9094bd1efc671e8ef7d001e"

KMETA = "kernel-meta"

# 获取kernel源码的地址
SRC_URI = " \
    git://github.com/raspberrypi/linux.git;name=machine;branch=${LINUX_RPI_BRANCH} \
    git://git.yoctoproject.org/yocto-kernel-cache;type=kmeta;name=meta;branch=yocto-5.10;destsuffix=${KMETA} \
    file://powersave.cfg \
    file://android-drivers.cfg \
    "

# 导入内核的公共设置
require linux-raspberrypi.inc

# 编译dtb时dtc的参数设置
KERNEL_DTC_FLAGS += "-@ -H epapr"
