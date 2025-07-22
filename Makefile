TARGET = mash
OBJ = mash.c input_parser.c helpers.c builtins.c executor.c

CFLAGS = -Wall -Wextra -Werror 

CC = gcc

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
clean:
	rm -f *-o
fclean: clean
	rm -f $(TARGET)
re: fclean all

