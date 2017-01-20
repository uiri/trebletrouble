#!/bin/sh

if [ ! -d /media/card ]; then
	echo "Temporary mount point [/media/card] not found"
	exit 1
fi

DEV=/dev/sdc2

if [ -b $DEV ]; then
	sudo mount $DEV /media/card
	sudo cp bbb/build/tmp/work/cortexa8hf-neon-poky-linux-gnueabi/trebletrouble/1.0-0/bin/trebletrouble /media/card/usr/bin/trebletrouble
	sudo sync
	sudo umount $DEV
	sudo sync
else
	echo "Block device $DEV does not exist"
fi
