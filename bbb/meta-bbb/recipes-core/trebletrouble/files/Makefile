CFLAGS=-Wall

LIBS=-lc -lasound -lsndfile -lm


bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: ugfx src/app.c bin/display.o bin/input.o bin/simpleAlsa.o bin/metronome.o bin/menu.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

ugfx: 
	$(MAKE) -C src/ugfxlib/menu

trebletrouble:	bin/trebletrouble

clean:
	./scripts/clean.sh
