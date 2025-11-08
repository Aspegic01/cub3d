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

int32_t xmove_player(t_map *map, int32_t value) {
	char	*cell;

	if (value == 0)
		return (map->player_position.x);
	int32_t x_index = map->player_position.x / map->cell_size;
	int32_t y_index = map->player_position.y / map->cell_size;
	cell = map->grid[y_index];
	int len = ft_strlen(cell);
	if (value > 0 && x_index + value < len && cell[x_index + 1] == '0')
		return map->player_position.x + value;
	if (value < 0 && x_index + value > 0 && cell[x_index - 1] == '0')
		return map->player_position.x + value;
	return (map->player_position.x);
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
		game->map->player_position.x = xmove_player(game->map, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		game->map->player_position.x = xmove_player(game->map, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		game->map->player_position.y += 1;//ymove_player(game->map, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		game->map->player_position.y -= 1;// ymove_player(game->map, -1);
}

static void	start(void *param)
{
	t_game	*game;

	game = param;
	minimap_render(game->map);
}

int	init_window(t_game *game)
{
	game->mlx = mlx_init(16 * 100, 9 * 100, WIN_TITLE, false);
	if (!game->mlx)
		return (ft_putstr_fd("Error\nFailed to initialize MLX\n", 2), -1);
	if (minimap_setup(game) != 0)
		return (-1);
	mlx_loop_hook(game->mlx, capture_keys, game);
	mlx_loop_hook(game->mlx, start, game);
	return (0);
}
