FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

# 为packagegroup-rpi-test附加一个运行时依赖: python3-sense-hat
# 参见: recipes-devtools/python/python3-sense-hat_2.2.0.bb
RDEPENDS_${PN} += "python3-sense-hat"

