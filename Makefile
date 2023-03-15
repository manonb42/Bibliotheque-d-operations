build ?= build


CC = gcc -Wall -pedantic -g

.PHONY: clean

all: $(build)/calc $(build)/test
clean: 
	rm -rf $(build)

$(build)/calc: calc_unbounded.c unbounded_int.c
	$(CC) $^ -o $@


$(build)/test: test_unbounded.c unbounded_int.c
	$(CC) $^ -o $@