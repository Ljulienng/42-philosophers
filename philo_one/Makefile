# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/02 16:21:17 by pganglof          #+#    #+#              #
#    Updated: 2020/08/16 02:47:39 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = 	./srcs/main.c 			\
		./srcs/ft_atoi.c 		\
		./srcs/ft_itoa.c		\
		./srcs/ft_strlen.c		\
		./srcs/philosopher_day.c\
		./srcs/print_message.c	\
		./srcs/thread.c

OBJS = $(SRCS:.c=.o)

NAME = philo_one
RM = rm -f
CC = clang
INCLUDES = ./includes/
CFLAGS = -Wall -Wextra -Werror  -pthread -g3 -I $(INCLUDES) 

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re