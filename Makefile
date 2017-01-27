CFLAGS=-Wall -std=gnu90 -g

LIBS=-lc -lasound -lsndfile -lm

bin/app: package/src/app.c bin/display.o bin/simpleAlsa.o bin/metronome.o bin/input.o bin/menu.o bin/tone.o bin/libfft.o bin/audio_recorder.o
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

bin/tone_test: package/src/tone_test.c bin/tone.o bin/libfft.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

display_test: bin/display_test 

tone_test: bin/tone_test

all:	app image
