CC=gcc
LFLAGS=-lncurses -g
BINDIR=./bin
SRCDIR=./src

server: $(SRCDIR)/server.c
	$(CC) $(LFLAGS) $^ -o $(BINDIR)/$@

client: $(SRCDIR)/client.c 
	$(CC) $(LFLAGS) $^ -o $(BINDIR)/$@

uinput: $(SRCDIR)/uinput_handler.c
	$(CC) $(LFLAGS) $^ -o $(BINDIR)/$@
