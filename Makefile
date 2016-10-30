CFLAGS=-Wall

LIBS=-lc -lasound

bin/app: package/src/app.c bin/display.o bin/loadWave.o bin/metronome.o bin/input.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

bin/%.o: package/src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

app:	bin/app

trebletrouble:
	. ./poky-krogoth/oe-init-build-env bbb/build/ && bitbake -f -c fetch trebletrouble && bitbake -f -c compile trebletrouble

image:
	. ./poky-krogoth/oe-init-build-env bbb/build/ && bitbake trebletrouble-image

install-srv:
	./install-srv.sh

install-bin:
	./install-bin.sh

sudo-format-sdcard:
	sudo ./bbb/meta-bbb/scripts/mk2parts.sh sdb

install-sdcard:
	OETMP=bbb/build/tmp ./bbb/meta-bbb/scripts/copy_rootfs.sh sdb trebletrouble

install-sdcard-boot:
	OETMP=bbb/build/tmp ./bbb/meta-bbb/scripts/copy_boot.sh sdb

bin/display_test: package/src/display_test.c bin/display.o
	$(CC) $(CFLAGS) $^ -o $@

display_test: bin/display_test 

all:	app image
