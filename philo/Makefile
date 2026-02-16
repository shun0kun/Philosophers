CC = cc
FLAG = -Wall -Wextra -Werror
SRCS = actions.c handle_forks.c main.c shared.c simulation.c threads.c time.c utils.c
OBJS = $(SRCS:.c=.o)
NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all
