SUMMARY = "Python module to control the Raspberry Pi Sense HAT used in the Astro Pi mission"
HOMEPAGE = "https://github.com/RPi-Distro/python-sense-hat"
SECTION = "devel/python"
LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://LICENCE.txt;md5=d80fe312e1ff5fbd97369b093bf21cda"

inherit setuptools3 pypi

# 从pypi安装一个sense-hat包
PYPI_PACKAGE = "sense-hat"

SRC_URI[md5sum] = "69929250cb72349a8a82edf2584b1d83"
SRC_URI[sha256sum] = "f000998d042d96ed722d459312e1bebd0107f9f3015cd34b3e4fabcab9c800af"

# 编译依赖
DEPENDS += " \
    jpeg \
    zlib \
    freetype \
    "

# 运行时依赖: python-numpy, python-rtimu, python-pillow
RDEPENDS_${PN} += " \
    ${PYTHON_PN}-numpy \
    ${PYTHON_PN}-rtimu \
    ${PYTHON_PN}-pillow \
    "
