# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 10:15:47 by mlabrirh          #+#    #+#              #
#    Updated: 2025/12/24 21:06:22 by mlabrirh         ###   ########.fr        #
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
       window.c movement.c \
       ./get_next_line/get_next_line.c \
       ./get_next_line/get_next_line_utils.c \
       ./vector/vecf_init.c ./vector/vecf_ops.c ./vector/veci_init.c ./vector/veci_ops.c \
       ./minimap/map.c ./engine/render.c ./engine/drawline.c map_parsing_utils.c texture_utils.c utils_c.c utils_s.c

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBMLX = ./include/minilibx
MLX = $(LIBMLX)/build/libmlx42.a
LIBS = $(MLX) -ldl -lglfw -lm
HEADERS = -I ./include -I $(LIBMLX)/include

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LIBFT) $(HEADERS) -o $(NAME)

$(MLX):
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

run: all
	./$(NAME) ./map.cub

clean:
	@rm -f $(OBJS)
	@rm -rf $(LIBMLX)/build
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: clean libmlx
