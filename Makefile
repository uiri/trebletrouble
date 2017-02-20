CFLAGS=-Wall -std=gnu90 -g
SD=sdb

LIBS=-lc -lasound -lsndfile -lm

bin/app: package/src/app.c bin/display.o bin/alsa.o bin/metronome.o bin/input.o bin/menu.o bin/tone.o bin/libfft.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

bin/%.o: package/src/%.c
	$(CC) $(LIBS) -c $(CFLAGS) $< -o $@

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
	sudo ./bbb/meta-bbb/scripts/mk2parts.sh $(SD)

install-sdcard:
	OETMP=bbb/build/tmp ./bbb/meta-bbb/scripts/copy_rootfs.sh $(SD) trebletrouble

install-sdcard-boot:
	OETMP=bbb/build/tmp ./bbb/meta-bbb/scripts/copy_boot.sh $(SD)

bin/display_test: package/src/display_test.c bin/display.o bin/gdisplay.o
	$(CC) $(CFLAGS) $^ -o $@

bin/tone_test: package/src/tone_test.c bin/tone.o bin/libfft.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

bin/record_test: package/src/record_test.c bin/tone.o bin/libfft.o bin/alsa.o
	$(CC) $(CFLAGS) $^ -o $@ -lm -lasound -lsndfile

display_test: bin/display_test 

bin/format_test: package/src/fileFormat_test.c bin/fileFormat.o bin/fileRead.o
	$(CC) $(CFLAGS) $^ -o $@

fileFormat_test: bin/format_test

tone_test: bin/tone_test

record_test: bin/record_test

screenshot.png: bin/display_test
	./bin/display_test
	pnmtopng screenshot.pnm > screenshot.png

all:	app image
