#include "../cub3d.h"

static void	render_map_cell(t_map *scene, t_v2 pos, uint32_t color)
{
	t_v2 start;
	t_v2 end;

	start = vec_new(pos.x, pos.y);
	end = vec_new(pos.x + scene->cell_size, pos.y + scene->cell_size);
	while (start.x < end.x)
	{
		start.y = pos.y;
		while (start.y < end.y)
		{
			mlx_put_pixel(scene->img, start.x, start.y, color);
			start.y++;
		}
		start.x++;
	}
}

void	minimap_render(t_map *scene)
{
	t_v2	iter;
	t_v2	pos;

	iter = vec_zero();
	while (iter.y < scene->height)
	{
		iter.x = 0;
		int line_len = ft_strlen(scene->grid[iter.y]);
		while (iter.x < scene->width)
		{
			pos = vec_scale(iter, scene->cell_size);
			if (iter.x < line_len && scene->grid[iter.y][iter.x] == '0')
				render_map_cell(scene, pos, 0xFFFFFFFF);
			else 
				render_map_cell(scene, pos, 0x333333);
			iter.x++;
		}
		iter.y++;
	}
}

static void	map_print(t_map *map)
{
	size_t i;
	size_t j;

	i = 0;
	while (map->grid[i])
	{
		j = 0;
		while (map->grid[i][j])
			j++;
		write(1, map->grid[i], j);
		write(1, "\n", 1);
		i++;
	}
}

int	minimap_setup(t_game *game)
{
	t_v2	map_size;
	t_v2	grid_dimensions;
	int32_t	instance_idx;

	grid_dimensions = vec_new(game->map->width, game->map->height);
	game->map->cell_size = game->mlx->width * 0.01;
	map_size = vec_scale(grid_dimensions, game->map->cell_size);
	game->map->img = mlx_new_image(game->mlx, map_size.x, map_size.y);
	if (!game->map->img)
		return (ft_putstr_fd("Error\nFailed to initialize image\n", 2), -1);
	instance_idx = mlx_image_to_window(game->mlx, game->map->img, 0, 0);
	if (instance_idx < 0)
		return (ft_putstr_fd("Error\nFailed to put image to window\n", 2), -1);
	map_print(game->map);
	vec_print("grid", grid_dimensions);
	vec_print("map_size", map_size);
	printf("cell_size{%d}\n", game->map->cell_size);
	return (0);
}
