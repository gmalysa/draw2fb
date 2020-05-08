DESCRIPTION = "Library for drawing to an array of memory in framebuffer-friendly formats"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=f98d55b2dbfee628e6c8b0445667d84d"

DEPENDS = "freetype liberation-fonts"

inherit pkgconfig

S = "${WORKDIR}"

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
	install -m 0755 ${WORKDIR}/libdraw2fb.a ${D}${libdir}/libdraw2fb.a
	install -m 0755 ${WORKDIR}/draw2fb.h ${D}${includedir}/draw2fb.h
}

FILES_${PN} = " \
	${libdir}/libdraw2fb.a \
	${includedir}/draw2fb.h \
"
