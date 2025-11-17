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
	int32_t	pos;
	int32_t	x_index;
	int		len;

	if (value == 0)
		return (map->player_position.x);
	y_index = map->player_position.y / map->cell_size;
	if (y_index < 0 || y_index >= map->height)
		return (map->player_position.x);
	cell = map->grid[y_index];
	len = ft_strlen(cell);
	pos = map->player_position.x + value;
	x_index = pos / map->cell_size;
	if (x_index < 0 || x_index >= len)
		return (map->player_position.x);
	if (cell[x_index] == '0')
		return (pos);
	return (map->player_position.x);
}

int32_t ymove_player(t_map *map, int32_t value) {
	char	*cell;
	int32_t	x_index;
	int32_t	y_index;
	int32_t	new_y_index;
	int		len;

	if (value == 0)
		return (map->player_position.y);
	x_index = map->player_position.x / map->cell_size;
	y_index = map->player_position.y / map->cell_size;
	new_y_index = (map->player_position.y + value) / map->cell_size;
	vec_print("player_pos", vec_new(x_index, y_index));
	if (new_y_index < 0 || new_y_index >= map->height)
		return (map->player_position.y);
	cell = map->grid[new_y_index];
	len = ft_strlen(cell);
	if (x_index < 0 || x_index >= len)
		return (map->player_position.y);
	if (cell[x_index] == '0')
		return (map->player_position.y + value);
	return (map->player_position.y);
}

void	capture_keys(void *param)
{
	t_game	*game;

	game = param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		close_window(game);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		game->map->player_position.x = clamp(game->map->img->width - game->map->cell_size, game->map->player_position.x, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		game->map->player_position.x = clamp(0, game->map->player_position.x, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		game->map->player_position.y = clamp(game->map->img->height - game->map->cell_size, game->map->player_position.y, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		game->map->player_position.y = clamp(0, game->map->player_position.y, -1);
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
	if (!game->canvas)
	{
		mlx_terminate(game->mlx);
		return (ft_putstr_fd("Error\nFailed to init image\n", 2), -1);
	}
	if (mlx_image_to_window(game->mlx, game->canvas, 0, 0) < 0)
	{
		mlx_terminate(game->mlx);
		return (ft_putstr_fd("Error\nFailed to put image to window\n", 2), -1);
	}
	if (minimap_setup(game) != 0)
	{
		mlx_terminate(game->mlx);
		return (-1);
	}
	mlx_loop_hook(game->mlx, capture_keys, game);
	mlx_loop_hook(game->mlx, start, game);
	return (0);
}
