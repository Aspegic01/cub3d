#include "../cub3d.h"

void	render_cell(t_map *scene, t_v2i pos, uint32_t color)
{
	t_v2i	start;
	t_v2i	end;

	start = veci_new(pos.x, pos.y);
	end = veci_new(pos.x + CELL_SIZE, pos.y + CELL_SIZE);
	while (start.y < end.y && end.y < scene->size.y)
	{
		start.x = pos.x;
		while (start.x < end.x && end.x < scene->size.x)
		{
			if (end.y - start.y == CELL_SIZE || end.x - start.x == CELL_SIZE)
				mlx_put_pixel(scene->img, start.x, start.y, color << 1);
			else
				mlx_put_pixel(scene->img, start.x, start.y, color);
			start.x++;
		}
		start.y++;
	}
}

void load_map_textures(t_map *map)
{
	map->tex_north = mlx_load_png(map->textures.north);
	map->tex_south = mlx_load_png(map->textures.south);
	map->tex_west  = mlx_load_png(map->textures.west);
	map->tex_east  = mlx_load_png(map->textures.east);

	if (!map->tex_north || !map->tex_south ||
		!map->tex_west || !map->tex_east)
	{
		ft_putstr_fd("Texture load error\n", 2);
		exit(1);
	}
}

int32_t	clamp(int32_t value, int32_t min, int32_t max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

void	draw_dirline(t_map *scene, t_v2f player_pos, uint32_t color)
{
	t_v2f	direction;
	player_pos.x += PLAYER_HALF;
	player_pos.y += PLAYER_HALF;

	direction = vecf_scale(scene->player.dir, (float_t)CELL_SIZE / 1.5);
	draw_line(scene->img, player_pos, vecf_add(player_pos, direction), color);
}

void	render_player(t_map *scene, uint32_t color)
{
	t_v2f	player_pos;
	t_v2i	grid_pos;
	t_v2i	pixel;
	t_v2i	offset;

	grid_pos = veci_new(scene->player.position.x - CELL_COUNT_HALF, scene->player.position.y - CELL_COUNT_HALF);
	grid_pos.x = clamp(grid_pos.x, 0, scene->width - 1);
	grid_pos.y = clamp(grid_pos.y, 0, scene->height - 1);
	player_pos.x = (scene->player.position.x - grid_pos.x) * CELL_SIZE;
	player_pos.y = (scene->player.position.y - grid_pos.y) * CELL_SIZE;
	offset.y = 0;
	while (offset.y < (int32_t)PLAYER_SIZE)
	{
		offset.x = 0;
		while (offset.x < (int32_t)PLAYER_SIZE)
		{
			pixel = veci_new(player_pos.x + offset.x, player_pos.y + offset.y);
			if (pixel.x >= 0 && pixel.x < scene->size.x && pixel.y >= 0 && pixel.y < scene->size.y)
				mlx_put_pixel(scene->img, pixel.x, pixel.y, color);
			offset.x++;
		}
		offset.y++;
	}
	draw_dirline(scene, player_pos, color);
}

void	minimap_render(t_game *game)
{
	t_v2i	grid_pos;
	t_v2i	grid_start;
	t_v2i	cell;
	int		line_len;

	grid_pos = veci_fromf(game->map->player.position);
	grid_start = veci_new(grid_pos.x - CELL_COUNT_HALF, grid_pos.y - CELL_COUNT_HALF);
	grid_start.x = clamp(grid_start.x, 0, game->map->width - 1);
	grid_start.y = clamp(grid_start.y, 0, game->map->height - 1);
	cell.y = 0;
	grid_pos.y = grid_start.y;
	while (cell.y < CELL_COUNT && grid_pos.y < game->map->height)
	{
		cell.x = 0;
		grid_pos.x = grid_start.x;
		line_len = ft_strlen(game->map->grid[grid_pos.y]);
		while (cell.x < CELL_COUNT && grid_pos.x < game->map->width)
		{
			if (grid_pos.x < line_len && game->map->grid[grid_pos.y][grid_pos.x] == '0')
				render_cell(game->map, veci_new(cell.x * CELL_SIZE, cell.y * CELL_SIZE), MAPFG);
			else
				render_cell(game->map, veci_new(cell.x * CELL_SIZE, cell.y * CELL_SIZE), MAPBG);
			grid_pos.x++;
			cell.x++;
		}
		grid_pos.y++;
		cell.y++;
	}
	render_player(game->map, 0x0000FFFF);
}

static void	map_print(t_map *map)
{
	size_t	i;
	size_t	j;

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
	t_v2i	grid_dimensions;
	int32_t	img_idx;

	grid_dimensions = veci_new(game->map->width, game->map->height);
	game->map->size = veci_new(CELL_SIZE * CELL_COUNT, CELL_SIZE * CELL_COUNT);
	game->map->img = mlx_new_image(game->mlx, game->map->size.x, game->map->size.y);
	if (!game->map->img)
		return (ft_putstr_fd("Error\nFailed to initialize image\n", 2), -1);
	game->map->position = veci_new(WIN_WIDTH * 0.01, WIN_HEIGHT * 0.01);
	img_idx = mlx_image_to_window(game->mlx, game->map->img,
			       game->map->position.x, game->map->position.y);
	if (img_idx < 0)
		return (ft_putstr_fd("Error\nFailed to put image to window\n", 2), -1);
	game->map->tex_north = mlx_load_png(game->map->textures.north);
	game->map->tex_south = mlx_load_png(game->map->textures.south);
	game->map->tex_west  = mlx_load_png(game->map->textures.west);
	game->map->tex_east  = mlx_load_png(game->map->textures.east);
	if (!game->map->tex_north || !game->map->tex_south ||
		!game->map->tex_west || !game->map->tex_east)
	{
		return (ft_putstr_fd("Error\nFailed to load textures\n", 2), -1);
	}
	map_print(game->map);
	veci_print("canvas", veci_new(WIN_WIDTH, WIN_HEIGHT));
	veci_print("grid", grid_dimensions);
	veci_print("map_size", game->map->size);
	veci_print("map_position", game->map->position);
	veci_print("cell_size", veci_new(CELL_SIZE, CELL_SIZE));
	vecf_print("player", game->map->player.position);
	return (0);
}
