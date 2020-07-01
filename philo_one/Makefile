SRCS = 	./srcs/main.c 			\
		./srcs/ft_atoi.c 		\
		./srcs/ft_isdigit.c 	\
		./srcs/ft_isspace.c 	\
		./srcs/ft_itoa.c		\
		./srcs/ft_strlen.c		\
		./srcs/ft_putstr.c 

OBJS = $(SRCS:.c=.o)

NAME = philo_one
RM = rm -f
CC = gcc
INCLUDES = ./includes/
CFLAGS = -Wall  -pthread -g  -fsanitize=address -g3 -I $(INCLUDES) 

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re