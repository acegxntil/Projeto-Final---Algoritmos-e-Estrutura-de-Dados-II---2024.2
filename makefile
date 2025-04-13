# Makefile for Movie Quotes Repository

CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c binary_search.c bst.c avl.c
OBJS = $(SRCS:.c=.o)
EXEC = movie_quotes_repo

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean