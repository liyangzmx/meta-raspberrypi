# 导入关于镜像的设置
include conf/machine/include/rpi-default-settings.inc
# 导入关于内核版本的设置, 也包括了从何处获取内核
include conf/machine/include/rpi-default-versions.inc
# 导入关于提供者的设置
include conf/machine/include/rpi-default-providers.inc

SOC_FAMILY = "rpi"
include conf/machine/include/soc-family.inc

IMAGE_FSTYPES ?= "tar.bz2 ext3 wic.bz2 wic.bmap"
WKS_FILE ?= "sdimage-raspberrypi.wks"

# 如果MACHINE_FEATURES中包含vc4graphics, 则将xserver-xorg-extension-glx添加到XSERVER
XSERVER = " \
    xserver-xorg \
    ${@bb.utils.contains("MACHINE_FEATURES", "vc4graphics", "xserver-xorg-extension-glx", "", d)} \
    ${@bb.utils.contains("MACHINE_FEATURES", "vc4graphics", "xf86-video-modesetting", "xf86-video-fbdev", d)} \
    "

# 见下文
RPI_KERNEL_DEVICETREE_OVERLAYS ?= " \
    overlays/at86rf233.dtbo \
    overlays/disable-bt.dtbo \
    overlays/dwc2.dtbo \
    overlays/gpio-ir.dtbo \
    overlays/gpio-ir-tx.dtbo \
    overlays/gpio-key.dtbo \
    overlays/hifiberry-amp.dtbo \
    overlays/hifiberry-dac.dtbo \
    overlays/hifiberry-dacplus.dtbo \
    overlays/hifiberry-digi.dtbo \
    overlays/justboom-both.dtbo \
    overlays/justboom-dac.dtbo \
    overlays/justboom-digi.dtbo \
    overlays/i2c-rtc.dtbo \
    overlays/imx219.dtbo \
    overlays/iqaudio-dac.dtbo \
    overlays/iqaudio-dacplus.dtbo \
    overlays/mcp2515-can0.dtbo \
    overlays/mcp2515-can1.dtbo \
    overlays/miniuart-bt.dtbo \
    overlays/pitft22.dtbo \
    overlays/pitft28-capacitive.dtbo \
    overlays/pitft28-resistive.dtbo \
    overlays/pitft35-resistive.dtbo \
    overlays/pps-gpio.dtbo \
    overlays/rpi-ft5406.dtbo \
    overlays/rpi-poe.dtbo \
    overlays/vc4-fkms-v3d.dtbo \
    overlays/vc4-kms-v3d.dtbo \
    overlays/w1-gpio.dtbo \
    overlays/w1-gpio-pullup.dtbo \
    "

# 见下文
RPI_KERNEL_DEVICETREE ?= " \
    bcm2708-rpi-zero.dtb \
    bcm2708-rpi-zero-w.dtb \
    bcm2708-rpi-b.dtb \
    bcm2708-rpi-b-rev1.dtb \
    bcm2708-rpi-b-plus.dtb \
    bcm2709-rpi-2-b.dtb \
    bcm2710-rpi-2-b.dtb \
    bcm2710-rpi-3-b.dtb \
    bcm2710-rpi-3-b-plus.dtb \
    bcm2711-rpi-4-b.dtb \
    bcm2711-rpi-400.dtb \
    bcm2708-rpi-cm.dtb \
    bcm2710-rpi-cm3.dtb \
    bcm2711-rpi-cm4.dtb \
    "

# 所有的设备树文件
KERNEL_DEVICETREE ??= " \
    ${RPI_KERNEL_DEVICETREE} \
    ${RPI_KERNEL_DEVICETREE_OVERLAYS} \
    "

# By default:
#
# * When u-boot is disabled use the "Image" format which can be directly loaded
#   by the rpi firmware.
#
# * When u-boot is enabled use the "uImage" format and the "bootm" command
#   within u-boot to load the kernel.
# U-Boot参数
# 启动boot命令为: bootm
KERNEL_BOOTCMD ??= "bootm"
# kernel的类型为: uImage
KERNEL_IMAGETYPE_UBOOT ??= "uImage"
# 如果不是用U-Boot启动内核, 应该用什么方式启动
KERNEL_IMAGETYPE_DIRECT ??= "zImage"
# 如果定义了RPI_USE_U_BOOT为1, 那么kernel镜像类型未uImage, 否则为zImage
KERNEL_IMAGETYPE ?= "${@bb.utils.contains('RPI_USE_U_BOOT', '1', \
        '${KERNEL_IMAGETYPE_UBOOT}', '${KERNEL_IMAGETYPE_DIRECT}', d)}"

# 平台相关的特性
MACHINE_FEATURES += "apm usbhost keyboard vfat ext2 screen touchscreen alsa bluetooth wifi sdio ${@bb.utils.contains('DISABLE_VC4GRAPHICS', '1', '', 'vc4graphics', d)}"

# Raspberry Pi has no hardware clock
# 使用回填的方式注释掉rtc的支持
MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

为要生成的映像安装的特定于计算机的软件包的列表
MACHINE_EXTRA_RRECOMMENDS += "kernel-modules udev-rules-rpi"

# 推荐的特定于机器的软件包的列表
MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS += "${@oe.utils.conditional('ENABLE_I2C', '1', 'kernel-module-i2c-dev kernel-module-i2c-bcm2708', '', d)}"
MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS += "${@oe.utils.conditional('ENABLE_IR', '1', 'kernel-module-gpio-ir kernel-module-gpio-ir-tx', '', d)}"

# 指定要使用的控制台
SERIAL_CONSOLES_CHECK ??= "${SERIAL_CONSOLES}"

# 启动文件的临时安装路径, 用于生成boot.img
# The name of the deploy directory for raspberry pi boot files.
# This variable is referred to by recipes fetching / generating the files.
BOOTFILES_DIR_NAME ?= "bootfiles"

# Set Raspberrypi splash image
SPLASH = "psplash-raspberrypi"

def make_dtb_boot_files(d):
    # Generate IMAGE_BOOT_FILES entries for device tree files listed in
    # KERNEL_DEVICETREE.
    alldtbs = d.getVar('KERNEL_DEVICETREE')
    imgtyp = d.getVar('KERNEL_IMAGETYPE')

    def transform(dtb):
        base = os.path.basename(dtb)
        if dtb.endswith('dtb'):
            # eg: whatever/bcm2708-rpi-b.dtb has:
            #     DEPLOYDIR file: bcm2708-rpi-b.dtb
            #     destination: bcm2708-rpi-b.dtb
            return base
        elif dtb.endswith('dtbo'):
            # overlay dtb:
            # eg: overlays/hifiberry-amp.dtbo has:
            #     DEPLOYDIR file: hifiberry-amp.dtbo
            #     destination: overlays/hifiberry-amp.dtbo
            return '{};{}'.format(base, dtb)

    return ' '.join([transform(dtb) for dtb in alldtbs.split(' ') if dtb])

# bb.utils.contains是在bitbake/lib/bb/utils.py中定义的
IMAGE_BOOT_FILES ?= "${BOOTFILES_DIR_NAME}/* \
                 ${@make_dtb_boot_files(d)} \
                 ${@bb.utils.contains('RPI_USE_U_BOOT', '1', \
                    '${KERNEL_IMAGETYPE} u-boot.bin;${SDIMG_KERNELIMAGE} boot.scr', \
                    '${KERNEL_IMAGETYPE};${SDIMG_KERNELIMAGE}', d)} \
                 "
# 制定在构建wic镜像时的依赖构建
do_image_wic[depends] += " \
    rpi-bootfiles:do_deploy \
    ${@bb.utils.contains('RPI_USE_U_BOOT', '1', 'u-boot:do_deploy', '',d)} \
    "

do_image_wic[recrdeps] = "do_build"

# The kernel image is installed into the FAT32 boot partition and does not need
# to also be installed into the rootfs.
RDEPENDS_${KERNEL_PACKAGE_NAME}-base = ""
