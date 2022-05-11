# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdouiri <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/09 13:45:22 by mdouiri           #+#    #+#              #
#    Updated: 2022/05/09 16:32:55 by mdouiri          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

RM = rm -f

SRC = pact.c pipex_utils.c

OBJS = ${SRC:.c=.o}

%.o: %.c
	${CC}	${CFLAGS} -c -o $@ $<

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean: 
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all      
