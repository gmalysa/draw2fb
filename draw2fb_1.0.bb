DESCRIPTION = "Library for drawing to an array of memory in framebuffer-friendly formats"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${S}/LICENSE;md5=f17cf300a93d4fa3f9f0cd408949ade3"

DEPENDS = "freetype liberation-fonts"

inherit pkgconfig

S = "${WORKDIR}/git"

SRC_URI = " \
	git://github.com/gmalysa/draw2fb.git;protocol=https;branch=master \
"

SRCREV = "${AUTOREV}"

do_compile() {
	oe_runmake -C ${S} libdraw2fb.a
}

do_install() {
	install -d ${D}${libdir}
	install -d ${D}${includedir}
	install -m 0755 ${S}/libdraw2fb.a ${D}${libdir}/libdraw2fb.a
	install -m 0755 ${S}/draw2fb.h ${D}${includedir}/draw2fb.h
}

FILES_${PN} = " \
	${libdir}/libdraw2fb.a \
	${includedir}/draw2fb.h \
"
