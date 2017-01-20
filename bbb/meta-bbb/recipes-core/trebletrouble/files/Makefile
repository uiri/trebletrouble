CFLAGS=-Wall

LIBS=-lc -lasound -lsndfile -lm


bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: src/app.c bin/display.o bin/input.o bin/simpleAlsa.o bin/metronome.o bin/menu.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

trebletrouble:	bin/trebletrouble

clean:
	./scripts/clean.sh
