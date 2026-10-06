CC = gcc
CFLAGS = -Wall -Wextra

SRCS = main.c error/error.c token/token.c lexer/lexer.c parser/parser.c node/node.c eval/eval.c
TARGET = sml

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lm

clean:
	rm -f $(TARGET)

.PHONY: all clean
