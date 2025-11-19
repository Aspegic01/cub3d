/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 11:30:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/17 11:23:23 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	close_window(t_game *game)
{
	if (game->mlx)
		mlx_close_window(game->mlx);
	return (0);
}

int32_t clamp(int32_t boundary, int32_t position, int32_t step)
{
	int32_t new_pos = position + step;
	if (step > 0)
	{
		if (new_pos >= boundary) return position;
		return new_pos;
	}
	else
	{
		if (new_pos <= boundary) return position;
		return new_pos;
	}
	return position;
}

int32_t xmove_player(t_map *map, int32_t value) {
	char	*cell;
	int32_t	y_index;

	if (value == 0)
		return (map->player_position.x);
	y_index = map->player_position.y / map->cell_size;
	cell = map->grid[y_index];
	int len = ft_strlen(cell);
	if (value > 0)
	{
		int32_t pos = map->player_position.x + value;
		int32_t x_index = pos / map->cell_size;
		if (x_index + 1 > len)
			return map->player_position.x;
		if (cell[x_index + value] == '0')
			return pos;
	}
	return map->player_position.x;
}

int32_t ymove_player(t_map *map, int32_t value) {
	char	*cell;

	if (value == 0)
		return (map->player_position.y);
	int32_t x_index = map->player_position.x / map->cell_size;
	int32_t y_index = map->player_position.y / map->cell_size;
	vec_print("player_pos", vec_new(x_index, y_index));
	cell = map->grid[x_index];
	if (value > 0 && y_index + value < map->height && cell[y_index + 1] == '0')
		return map->player_position.y + value;
	if (value < 0 && y_index - 1 > 0 && cell[y_index - 1] == '0')
		return map->player_position.y + value;
	return (map->player_position.y);
}

void	capture_keys(void *param)
{
	t_game	*game;

	game = param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		close_window(game);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
	{
		int32_t box = game->map->cell_size * 0.5;
		int32_t x = (game->map->player_position.x + box + 1) / game->map->cell_size;
		int32_t y1 = (game->map->player_position.y) / game->map->cell_size;
		int32_t y2 = (game->map->player_position.y + box) / game->map->cell_size;
		if (game->map->grid[y1][x] == '0' && game->map->grid[y2][x] == '0') {
			game->map->player_position.x += 1;
		}
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
	{
		int32_t box = game->map->cell_size * 0.5;
		int32_t x = (game->map->player_position.x - 1) / game->map->cell_size;
		int32_t y1 = game->map->player_position.y / game->map->cell_size;
		int32_t y2 = (game->map->player_position.y + box) / game->map->cell_size;
		if (game->map->grid[y1][x] == '0' && game->map->grid[y2][x] == '0') {
			game->map->player_position.x -= 1;
		}
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
	{
		int32_t box = game->map->cell_size * 0.5;
		int32_t x1 = game->map->player_position.x / game->map->cell_size;
		int32_t x2 = (game->map->player_position.x + box) / game->map->cell_size;
		int32_t y = (game->map->player_position.y + box + 1) / game->map->cell_size;
		if (game->map->grid[y][x1] == '0' &&  game->map->grid[y][x2] == '0') {
			game->map->player_position.y += 1;
		}
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
	{
		int32_t box = game->map->cell_size * 0.5;
		int32_t x1  = game->map->player_position.x / game->map->cell_size;
		int32_t x2 = (game->map->player_position.x + box) / game->map->cell_size;
		int32_t y = (game->map->player_position.y - 1) / game->map->cell_size;
		if (game->map->grid[y][x1] == '0' && game->map->grid[y][x2] == '0') {
			game->map->player_position.y -= 1;
		}
	}
}

static void	start(void *param)
{
	t_game	*game;

	game = param;
	render_game(game);
	minimap_render(game->map);
}

int	init_window(t_game *game)
{
	game->mlx = mlx_init(16 * 100, 9 * 100, WIN_TITLE, false);
	if (!game->mlx)
		return (ft_putstr_fd("Error\nFailed to initialize MLX\n", 2), -1);
	game->canvas = mlx_new_image(game->mlx, game->mlx->width, game->mlx->height);
	if (!game->mlx)
		return (ft_putstr_fd("Error\nFailed to init image\n", 2), -1);
	if (mlx_image_to_window(game->mlx, game->canvas, 0, 0) < 0)
		return (ft_putstr_fd("Error\nFailed to put image to window\n", 2), -1);
	if (minimap_setup(game) != 0)
		return (-1);
	mlx_loop_hook(game->mlx, capture_keys, game);
	mlx_loop_hook(game->mlx, start, game);
	return (0);
}
