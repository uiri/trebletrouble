SUMMARY = "An image which boots into the TrebleTrouble application"
HOMEPAGE = "http://uottawa.ca"

IMAGE_LINGUAS = "en-us"

require console-image.bb

ALSA_TOOLS = " \ 
    alsa-lib \ 
    alsa-tools \ 
    alsa-utils \ 
 " 

IMAGE_INSTALL += " \
    ${ALSA_TOOLS} \
    trebletrouble \
 "

export IMAGE_BASENAME = "trebletrouble-image"
