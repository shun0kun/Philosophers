CC = cc
CFLAGS = -Wall -Wextra -Werror
DIR_SRCS = srcs/
DIR_OBJS = objs/
SRCS = actions.c characters.c fork.c main.c mark.c shared.c simulation.c stop.c time.c utils.c
OBJS = $(SRCS:.c=.o)
SRCS_PATH = $(addprefix $(DIR_SRCS), $(SRCS))
OBJS_PATH = $(addprefix $(DIR_OBJS), $(OBJS))
NAME = philo

all: $(NAME)

$(NAME): $(OBJS_PATH)
	$(CC) $(CFLAGS) $(OBJS_PATH) -o $(NAME)

$(DIR_OBJS)%.o: $(DIR_SRCS)%.c | $(DIR_OBJS)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_OBJS):
	mkdir -p $(DIR_OBJS)

clean:
	rm -rf $(DIR_OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
