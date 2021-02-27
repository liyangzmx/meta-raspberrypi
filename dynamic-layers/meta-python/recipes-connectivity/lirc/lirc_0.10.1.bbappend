# 标识SRC_URI_append_${MACHINE}的位置从FILESEXTRAPATHS_prepend处开始找
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

# 额外对lirc打补丁, 并添加一个启动服务配置文件
SRC_URI_append_rpi = " \
	file://lirc-gpio-ir-0.10.patch \
        file://lircd.service \
"
