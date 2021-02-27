# RaspberryPi BSP default providers

# 设置内核的配方(virtual/kernel)为: linux-raspberrypi
PREFERRED_PROVIDER_virtual/kernel ??= "linux-raspberrypi"
# 设置其它对象的提供者
PREFERRED_PROVIDER_virtual/xserver ?= "xserver-xorg"
PREFERRED_PROVIDER_virtual/egl ?= "${@bb.utils.contains("MACHINE_FEATURES", "vc4graphics", "mesa", "userland", d)}"
PREFERRED_PROVIDER_virtual/libgles2 ?= "${@bb.utils.contains("MACHINE_FEATURES", "vc4graphics", "mesa", "userland", d)}"
PREFERRED_PROVIDER_virtual/libgl ?= "${@bb.utils.contains("MACHINE_FEATURES", "vc4graphics", "mesa", "mesa-gl", d)}"
PREFERRED_PROVIDER_virtual/mesa ?= "${@bb.utils.contains("MACHINE_FEATURES", "vc4graphics", "mesa", "mesa-gl", d)}"
PREFERRED_PROVIDER_virtual/libgbm ?= "${@bb.utils.contains("MACHINE_FEATURES", "vc4graphics", "mesa", "mesa-gl", d)}"
PREFERRED_PROVIDER_jpeg ?= "jpeg"

PREFERRED_PROVIDER_virtual/libomxil ?= "userland"
VIRTUAL-RUNTIME_libomxil = "userland"

# 设置提供u-boot-default-script的配方为: rpi-u-boot-scr
PREFERRED_PROVIDER_u-boot-default-script ??= "rpi-u-boot-scr"
