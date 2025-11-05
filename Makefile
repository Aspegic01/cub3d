# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 10:15:47 by mlabrirh          #+#    #+#              #
#    Updated: 2025/10/10 10:17:36 by mlabrirh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRCS = main.c \
       map_parsing.c \
       texture_loader.c \
       player_init.c \
       utils.c \
       window.c \
       ./get_next_line/get_next_line.c \
       ./get_next_line/get_next_line_utils.c \
       ./vector/vec_init.c ./vector/vec_ops.c

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBMLX = ./include/minilibx
LIBS = $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
HEADERS = -I ./include -I $(LIBMLX)/include

OBJS = $(SRCS:.c=.o)

all: libmlx $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LIBFT) $(HEADERS) -o $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

clean:
	@rm -f $(OBJS)
	@rm -rf $(LIBMLX)/build
	@make clean -C $(LIBFT_DIR)
fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
re: fclean all

.PHONY: clean
