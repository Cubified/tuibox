all: tuibox

CC=cc

LIBS=-lm
CFLAGS=-O3 -pipe
DEBUGCFLAGS=-Og -pipe -g

.PHONY: tuibox
tuibox:
	$(CC) demos/demo_basic.c -o demos/demo_basic $(LIBS) $(CFLAGS)
	$(CC) demos/demo_bounce.c -o demos/demo_bounce $(LIBS) $(CFLAGS)
	$(CC) demos/demo_drag.c -o demos/demo_drag $(LIBS) $(CFLAGS)
