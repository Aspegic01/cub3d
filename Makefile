# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 10:15:47 by mlabrirh          #+#    #+#              #
#    Updated: 2025/08/28 13:46:53 by mlabrirh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c \
       map_parsing.c \
       texture_loader.c \
       player_init.c \
       utils.c \
       ./get_next_line/get_next_line.c \
       ./get_next_line/get_next_line_utils.c
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)
clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)
fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)
re: fclean all

.PHONY: all clean fclean re