CFLAGS=-Wall

LIBS=-lc -lasound -lm

bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: src/app.c bin/display.o bin/tone.o bin/pitch.o bin/libfft.o bin/loadWave.o bin/metronome.o bin/input.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

trebletrouble:	bin/trebletrouble

clean:
	./scripts/clean.sh
