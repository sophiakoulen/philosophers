NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

ifeq ($(DEBUG), addr)
CFLAGS += -g3 -fsanitize=address
endif

ifeq ($(DEBUG), thread)
CFLAGS += -g3 -fsanitize=thread
endif

HEADERS = philo.h
FILES = main parsing routine utils init supervisor destroy
INCS = .

SRCS = $(addsuffix .c, $(FILES))
OBJS = $(addsuffix .o, $(addprefix objs/, $(FILES)))

objs/%.o: %.c
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
