NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
ifdef DEBUG
CFLAGS += -g3 -fsanitize=thread
endif

HEADERS = philo.h
FILES = main parsing routine utils check
INCS = .

SRCS = $(addsuffix .c, $(addprefix srcs/, $(FILES)))
OBJS = $(addsuffix .o, $(addprefix objs/, $(FILES)))

objs/%.o: srcs/%.c
	mkdir -p objs
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCS)

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -rf objs

fclean: clean
	rm -f $(NAME)

re: fclean all
