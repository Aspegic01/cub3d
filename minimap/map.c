#include "../cub3d.h"

static void	render_cell(t_map *scene, t_v2i pos, uint32_t color)
{
	t_v2i	start;
	t_v2i	end;

	start = veci_new(pos.x, pos.y);
	end = veci_new(pos.x + CELL_SIZE, pos.y + CELL_SIZE);
	while (start.y < end.y)
	{
		start.x = pos.x;
		while (start.x < end.x)
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

static t_v2f	get_player_center(t_player *p)
{
	t_v2f that = vecf_scale(vecf_new(p->position.x, p->position.y), CELL_SIZE);
	that.x += PLAYER_HALF;
	that.y += PLAYER_HALF;
	return (that);
}

static void	draw_dirline(t_map *scene, uint32_t color)
{
	t_v2f	direction;
	t_v2f	center;

	center = get_player_center(&scene->player);
	direction = vecf_scale(scene->player.dir, CELL_SIZE);
	draw_line(scene->img, center, vecf_add(center, direction), color);
}

bool	at_wall(t_map *map, float_t offsetx, float_t offsety)
{
	int32_t	x;
	int32_t	y;

	x = floorf(offsetx);
	y = floorf(offsety);
	if (y <= 0 || y >= map->height || x <= 0 || x >= map->width)
		return (true);
	if (x >= (int32_t)ft_strlen(map->grid[y]))
		return (true);
	return (map->grid[y][x] != '0');
}

void	draw_fov(t_game *game, t_map *scene, t_player *player, uint32_t color)
{
	uint32_t x = 0;
	while (x < game->canvas->width)
	{
		float_t camerax = 2.0 * (x / (float_t)game->canvas->width) - 1.0;
		float_t perp_wall_dist;
		uint32_t hit = 0;
		uint32_t side = 0;
		t_v2f map = vecf_new(player->position.x, player->position.y);
		t_v2i step = veci_zero();
		t_v2f ray_dir = vecf_add(player->dir, vecf_scale(player->plane, camerax));
		t_v2f side_dist = vecf_zero();
		t_v2f delta_dist = vecf_zero();

		if (ray_dir.x == 0.0)
			step.x = 1e30;
		else
			step.x = fabs(1 / ray_dir.x);
		if (ray_dir.y == 0)
			step.y = 1e30;
		else
			step.y = fabs(1 / ray_dir.y);

		if (ray_dir.x < 0)
		{
			step.x = -1;
			side_dist.x = (player->position.x - map.x) * delta_dist.x;
		} else {
			step.x = 1;
			side_dist.x = ((map.x + 1.0) - player->position.x) * delta_dist.x;
		}
		if (ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = (player->position.y - map.y) * delta_dist.y;
		} else {
			step.y = 1;
			side_dist.y = ((map.y + 1.0) - player->position.y) * delta_dist.y;
		}
		while (hit == 0)
		{
			if (side_dist.x < side_dist.y)
			{
				side_dist.x += delta_dist.x;
				map.x += step.x;
				side = 0;
			} else {
				side_dist.y += delta_dist.y;
				map.y += step.y;
				side = 1;
			}
			if (at_wall(scene, map.x, map.y))
				hit = 1;
		}
		
		if (side == 0)
			perp_wall_dist = (side_dist.x - delta_dist.x);
		else
			perp_wall_dist = (side_dist.y - delta_dist.y);
		draw_line(game->map->img, player->position, vecf_new(map.x, map.y), color);
		(void)perp_wall_dist;
		// int32_t line_height = (int32_t)game->canvas->height / perp_wall_dist;
		// int32_t draw_start = -line_height / 2 + game->canvas->height / 2;
		// if (draw_start < 0)
		// 	draw_start = 0;
		// int32_t draw_end = line_height / 2 + game->canvas->height / 2;
		// if (draw_end >= (int32_t)game->canvas->height)
		// 	draw_end = game->canvas->height - 1;
		// int32_t y = draw_start;
		// while (y < draw_end)
		// {
		// 	mlx_put_pixel(game->canvas, x, y, color);
		// 	y++;
		// }
		x++;
	}
}

static void	render_player(t_game *game, t_map *scene, uint32_t color)
{
	t_v2f	start;
	t_v2f	end;

	start = vecf_scale(vecf_new(scene->player.position.x, scene->player.position.y), CELL_SIZE);
	end = vecf_new(start.x + PLAYER_SIZE, start.y + PLAYER_SIZE);
	while (start.y <= end.y)
	{
		start.x = scene->player.position.x * CELL_SIZE;
		while (start.x <= end.x)
		{
			mlx_put_pixel(scene->img, start.x, start.y, color);
			start.x++;
		}
		start.y++;
	}
	draw_fov(game, scene, &scene->player, 0xFF00FFFF);
	draw_dirline(scene, color);
}

void	minimap_render(t_game *game)
{
	t_v2i	pos;
	int		line_len;

	pos = veci_zero();
	while (pos.y < game->map->height)
	{
		pos.x = 0;
		line_len = ft_strlen(game->map->grid[pos.y]);
		while (pos.x < game->map->width)
		{
			if (pos.x < line_len && game->map->grid[pos.y][pos.x] == '0')
				render_cell(game->map, veci_scale(pos, CELL_SIZE), MAPFG);
			else
				render_cell(game->map, veci_scale(pos, CELL_SIZE), MAPBG);
			pos.x++;
		}
		pos.y++;
	}
	render_player(game, game->map, 0x0000FFFF);
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
	t_v2i	map_size;
	t_v2i	grid_dimensions;
	int32_t	img_idx;

	grid_dimensions = veci_new(game->map->width, game->map->height);
	map_size = veci_scale(grid_dimensions, CELL_SIZE);
	game->map->img = mlx_new_image(game->mlx, map_size.x, map_size.y);
	if (!game->map->img)
		return (ft_putstr_fd("Error\nFailed to initialize image\n", 2), -1);
	game->map->position = veci_new(WIN_WIDTH * 0.01, WIN_HEIGHT * 0.01);
	img_idx = mlx_image_to_window(game->mlx, game->map->img,
			game->map->position.x, game->map->position.y);
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
