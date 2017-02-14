CFLAGS=-Wall

LIBS=-lc -lasound -lsndfile -lm


bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: src/ugfx/.build/libugfx.a src/app.c bin/display.o bin/input.o bin/simpleAlsa.o bin/metronome.o bin/menu.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

src/ugfx/.builld/libugfx.a:
	$(MAKE) -C src/ugfx

trebletrouble:	bin/trebletrouble

clean:
	./scripts/clean.sh
