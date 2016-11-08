CFLAGS=-Wall

bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: src/app.c bin/display.o
	$(CC) $(CFLAGS) $^ -o $@

trebletrouble:	bin/trebletrouble

clean:
	./scripts/clean.sh
