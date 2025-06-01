# Makefile
PROGRAM = hello

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).c
	$(CC) $(PROGRAM).c -o $(PROGRAM)

install: all
	install -m 0755 $(PROGRAM) $(DESTDIR)/usr/local/bin

clean:
	rm -f $(PROGRAM)


	