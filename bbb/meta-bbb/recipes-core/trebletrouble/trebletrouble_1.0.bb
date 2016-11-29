SUMMARY = "TrebleTrouble application"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://LICENSE;md5=84dcc94da3adb52b53ae4fa38fe49e5d"

SRC_URI = "file://LICENSE \
	   file://Makefile \
	   file://scripts/* \
	   file://src/* \
	   file://share/* \
	  "
ALSA_TOOLS = " \ 
    alsa-plugins \
    alsa-state \
    alsa-lib \ 
    alsa-lib-native \ 
    alsa-tools \ 
    alsa-utils \ 
 " 


DEPENDS = "${ALSA_TOOLS}"

PR = "0"
S = "${WORKDIR}"

do_compile() {
    if [ -d bin ]; then
        make trebletrouble
    else
        mkdir bin && make trebletrouble
    fi
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 bin/trebletrouble ${D}${bindir}
    install -d ${D}${servicedir}/trebletrouble
    install -m 0644 share/* ${D}${servicedir}/trebletrouble/
}

FILES_${PN} = "${bindir} \
	       ${servicedir}/trebletrouble"
