#include "../cub3d.h"

static void	render_map_cell(t_map *scene, t_v2i pos, uint32_t color, bool walkable)
{
	t_v2i start;
	t_v2i end;

	start = veci_new(pos.x, pos.y);
	end = veci_new(pos.x + CELL_SIZE, pos.y + CELL_SIZE);
	while (start.y < end.y)
	{
		start.x = pos.x;
		while (start.x < end.x)
		{
			if (walkable && (end.y - start.y == CELL_SIZE|| end.x - start.x == CELL_SIZE))
				mlx_put_pixel(scene->img, start.x, start.y, 0x000000FF);
			else
				mlx_put_pixel(scene->img, start.x, start.y, color);
			start.x++;
		}
		start.y++;
	}
}

static void drawDirLine(t_map *scene, uint32_t color) {
	t_v2f direction;
	t_v2f center;
	t_v2f pos;

	pos = scene->player.position;
	center = vecf_new(pos.x + PLAYER_HALF, pos.y + PLAYER_HALF);
	direction = vecf_scale(scene->player.dir, CELL_SIZE);
	draw_line(scene->img, center, vecf_add(center, direction), color);
}

// void draw_fov(t_map *scene, uint32_t color)
// {
// 	t_v2f pos = scene->player.position;
// 	int32_t line_width = 4;
// 	int32_t window_width = 16 * 100;
// 	int32_t ray_count = window_width / line_width;
// 	float_t rotationSpeed = 2 * (M_PI / 180);
// 	float_t rotationAngle = scene->player.dir.x * rotationSpeed;
// }

static void	render_player(t_map *scene,  uint32_t color)
{
	t_v2i start;
	t_v2i end;

	start = veci_new(scene->player.position.x, scene->player.position.y);
	end = veci_new(start.x + PLAYER_SIZE, start.y + PLAYER_SIZE);
	while (start.y <= end.y)
	{
		start.x = (int32_t)scene->player.position.x;
		while (start.x <= end.x)
		{
			mlx_put_pixel(scene->img, start.x, start.y, color);
			start.x++;
		}
		start.y++;
	}

	drawDirLine(scene, color);
	// draw_fov(scene, color);
}

void	minimap_render(t_map *scene)
{
	t_v2i pos;
	int line_len;
	
	pos = veci_zero();
	while (pos.y < scene->height)
	{
		pos.x = 0;
		line_len = ft_strlen(scene->grid[pos.y]);
		while (pos.x < scene->width)
		{
			if (pos.x < line_len && scene->grid[pos.y][pos.x] == '0')
				render_map_cell(scene, veci_scale(pos, CELL_SIZE), 0xFFFFFFFF, true);
			else
				render_map_cell(scene, veci_scale(pos, CELL_SIZE), 0x333333FF, false);
			pos.x++;
		}
		pos.y++;
	}

	render_player(scene, 0x0000FFFF);
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
	t_v2i	map_size;
	t_v2i	grid_dimensions;
	int32_t	img_idx;

	grid_dimensions = veci_new(game->map->width, game->map->height);
	map_size = veci_scale(grid_dimensions, CELL_SIZE);
	game->map->img = mlx_new_image(game->mlx, map_size.x, map_size.y);
	if (!game->map->img)
		return (ft_putstr_fd("Error\nFailed to initialize image\n", 2), -1);
	game->map->position = veci_new(WIN_WIDTH * 0.01, WIN_HEIGHT * 0.01);
	game->map->player.position = vecf_scale(game->map->player.position, CELL_SIZE);
	img_idx = mlx_image_to_window(game->mlx, game->map->img, game->map->position.x, game->map->position.y);
	if (img_idx < 0)
		return (ft_putstr_fd("Error\nFailed to put image to window\n", 2), -1);
	map_print(game->map);
	veci_print("canvas", veci_new(WIN_WIDTH, WIN_HEIGHT));
	veci_print("grid", grid_dimensions);
	veci_print("map_size", map_size);
	veci_print("map_position", game->map->position);
	veci_print("cell_size", veci_new(CELL_SIZE, CELL_SIZE));
	vecf_print("player", game->map->player.position);
	return (0);
}
