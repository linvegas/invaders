CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c2x
LDFLAGS = -lraylib
EXEC    = invaders
SRC     = main.c

# all: $(EXEC)

$(EXEC): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(EXEC) $(SRC)
