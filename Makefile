CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c2x -I./raylib/include -L./raylib/lib
LDFLAGS = -l:libraylib.a -lm
EXEC    = invaders
SRC     = main.c

# all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(LDFLAGS)
