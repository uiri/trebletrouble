CFLAGS=-Wall

LIBS=-lc -lasound


bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: src/app.c bin/display.o bin/input.o bin/loadWave.o bin/metronome.o
	$(CC) $(CFLAGS) $^ -o $@(LIBS)

trebletrouble:	bin/trebletrouble

clean:
	./scripts/clean.sh
