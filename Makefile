IDIR=./include
ODIR=./obj
SDIR=./src

CC=gcc
CFLAGS=-Wall -g -O2 -I$(IDIR)
LIBS=-lm

DEPS=$(wildcard $(IDIR)/*.h)
OBJS=$(patsubst $(IDIR)/%.h,$(ODIR)/%.o,$(DEPS))
SRCS=$(patsubst $(IDIR)/%.h,$(SDIR)/%.c,$(DEPS))

main: main.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c $(IDIR)/%.h
	mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR) main.o main
