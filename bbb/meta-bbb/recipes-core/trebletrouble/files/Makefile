CFLAGS="-Wall"

LIBS=-lc -lasound -lm

bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: src/app.c bin/display.o bin/tone.o bin/loadWave.o bin/metronome.o
	  $(CC) $(CFLAGS) $< bin/display.o bin/tone.o bin/loadWave.o bin/metronome.o -o $@ $(LIBS)

trebletrouble:	bin/trebletrouble

clean:
	./scripts/clean.sh
