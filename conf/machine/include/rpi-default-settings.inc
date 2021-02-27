# RaspberryPi BSP default settings

# 所有图像应继承的类的列表
IMAGE_CLASSES += "sdcard_image-rpi"

# 如何测试编译生成的镜像
# RPI kernel has errors of its own which should be filtered
# therefore use parselogs_rpi test instead of parselogs from oe-core
#
DEFAULT_TEST_SUITES_append_rpi = " parselogs_rpi"
DEFAULT_TEST_SUITES_remove_rpi = " parselogs"
