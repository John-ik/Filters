all: debug

test: debug
	./debug
	echo "Test success"

debug: example/main.c filters.c
	gcc -g -o debug example/main.c filters.c

clean:
	rm -f *.o debug