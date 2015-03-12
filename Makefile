
SRCDIR=src
OBJDIR=obj
HDRDIR=include

EXEC=2048
CC=gcc
CFLAGS=-W -Wall -Wextra -I $(HDRDIR) -g
LDFLAGS=

_DEPS=grid.h game.h ia.h ui.h 
_SRC=$(EXEC).c grid.c game.c ia.c ui.c
_OBJ=$(_SRC:.c=.o)
DEPS=$(patsubst %,$(HDRDIR)/%,$(_DEPS))
OBJ=$(patsubst %,$(OBJDIR)/%,$(_OBJ))
SRC=$(patsubst %,$(SRCDIR)/%,$(_SRC))

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJDIR)/$(EXEC).o: $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf $(OBJDIR)/*.o

mproper: clean
	rm -rf $(EXEC)
