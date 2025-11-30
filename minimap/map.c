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
	return (vecf_new(p->position.x + PLAYER_HALF, p->position.y + PLAYER_HALF));
}

static void	draw_dirline(t_map *scene, uint32_t color)
{
	t_v2f	direction;
	t_v2f	center;

	center = get_player_center(&scene->player);
	direction = vecf_scale(scene->player.dir, CELL_SIZE);
	draw_line(scene->img, center, vecf_add(center, direction), color);
}

// void	draw_fov(t_map *scene, t_player *player, uint32_t color)
// {
// 	// int32_t	ray_count;
// 	float_t	ray_angle;
// 	float_t	camerax;
// 	uint32_t	i;
// 	t_v2f	wall_hit;
// 	t_v2f	h_hit;
// 	t_v2f	first_inter;
// 	t_v2f	next_h_hit;
// 	t_v2f	move_step;
// 	t_v2f	ray_dir;
// 	bool	found_horizontal_wall;
//
// 	// t_v2f	center;
// 	wall_hit = vecf_zero();
// 	// center = get_player_center(&scene->player);
// 	// ray_angle = -(FOV_ANGLE / 2);
// 	// ray_count = scene->width / 2;
// 	// bool is_facing_down = ray_angle > 0 && ray_angle < M_PI;
// 	// bool is_facing_up = !is_facing_down;
// 	// bool is_facing_right = ray_angle < 0.5 * M_PI || ray_angle > 1.5 * M_PI;
// 	// bool is_facing_left = !is_facing_right;
// 	i = 0;
//
// 	// ray_dir = vecf_scale(scene->player.dir, CELL_SIZE * 2);
// 	// ray_dir = vecf_rot(scene->player.dir, ray_angle);
// 	// draw_line(scene->img, center, vecf_add(center, ray_dir), color);
// 	while (i < scene->img->width)
// 	{
// 		wall_hit = vecf_zero();
// 		found_horizontal_wall = false;
// 		h_hit = vecf_zero();
// 		camerax = 2.0 * (i / (float_t)scene->img->width) - 1.0;
// 		ray_dir = vecf_add(player->dir, vecf_scale(player->plane, camerax));
// 		ray_angle = atan2f(ray_dir.y, ray_dir.x);
// 		bool is_facing_down = ray_angle > 0 && ray_angle < M_PI;
// 		// bool is_facing_right = ray_angle < 0.5 * M_PI || ray_angle > 1.5 * M_PI;
// 		first_inter = vecf_zero();
// 		if (!is_facing_down)
// 			first_inter.y = floorf(player->position.y / CELL_SIZE) * CELL_SIZE
// 				- 1;
// 		if (is_facing_down)
// 			first_inter.y = floorf(player->position.y / CELL_SIZE) * CELL_SIZE
// 				+ CELL_SIZE;
// 		first_inter.x = player->position.x + (first_inter.y
// 				- player->position.y) / tanf(ray_angle);
// 		next_h_hit = vecf_from(first_inter);
// 		move_step = vecf_zero();
// 		if (player->looking_up)
// 			move_step.y = (int32_t)CELL_SIZE * -1;
// 		else
// 			move_step.y = (int32_t)CELL_SIZE;
// 		move_step.x = move_step.y / tanf(ray_angle);
// 		// move_step.x = ceilf(move_step.x);
// 		// NOTE: may need to check for the map offset
// 		while (next_h_hit.x <= scene->img->width && next_h_hit.x >= 0
// 			&& next_h_hit.y <= scene->img->height && next_h_hit.y >= 0)
// 		{
// 			if (at_wall(scene, next_h_hit.x, next_h_hit.y))
// 			{
// 				found_horizontal_wall = true;
// 				h_hit.x = next_h_hit.x;
// 				h_hit.y = next_h_hit.y;
// 				break ;
// 			}
// 			else
// 			{
// 				next_h_hit = vecf_add(next_h_hit, move_step);
// 			}
// 		}
//
// 		wall_hit.x = h_hit.x;
// 		wall_hit.y = h_hit.y;
// 		(void)found_horizontal_wall;
// 		draw_line(scene->img, player->position, wall_hit, color);
//
// 		// ray_dir = vecf_scale(scene->player.dir, CELL_SIZE * 2);
// 		// ray_dir = vecf_rot(ray_dir, ray_angle);
// 		// draw_line(scene->img, center, vecf_add(center, ray_dir), color);
// 		ray_angle += FOV_ANGLE / scene->img->width;
// 		i++;
// 	}
// }

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
		t_v2f map = vecf_new(player->position.x / CELL_SIZE, player->position.y / CELL_SIZE);
		t_v2i step = veci_zero();
		t_v2f ray_dir = vecf_add(player->dir, vecf_scale(player->plane, camerax));
		t_v2f side_dist = vecf_zero();
		t_v2f delta_dist = vecf_new(fabsf(1 / ray_dir.x), fabsf(1 / ray_dir.y));

		if (ray_dir.x < 0)
		{
			step.x = -1;
			side_dist.x = (player->position.x - map.x * CELL_SIZE) * delta_dist.x / CELL_SIZE;
		} else {
			step.x = 1;
			side_dist.x = ((map.x + 1.0) * CELL_SIZE - player->position.x) * delta_dist.x / CELL_SIZE;
		}
		if (ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = (player->position.y - map.y * CELL_SIZE) * delta_dist.y / CELL_SIZE;
		} else {
			step.y = 1;
			side_dist.y = ((map.y + 1.0) * CELL_SIZE - player->position.y) * delta_dist.y / CELL_SIZE;
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
			// perp_wall_dist = (side_dist.x - delta_dist.x);
			perp_wall_dist = (map.x - player->position.x / CELL_SIZE + (1.0 - step.x) / 2) / ray_dir.x;
		else
			// perp_wall_dist = (side_dist.y - delta_dist.y);
			perp_wall_dist = (map.y - player->position.y / CELL_SIZE + (1.0 - step.y) / 2) / ray_dir.y;
		int32_t line_height = (int32_t)game->canvas->height / perp_wall_dist;
		int32_t draw_start = -line_height / 2 + game->canvas->height / 2;
		if (draw_start < 0)
			draw_start = 0;
		int32_t draw_end = line_height / 2 + game->canvas->height / 2;
		if (draw_end >= (int32_t)game->canvas->height)
			draw_end = game->canvas->height - 1;
		int32_t y = draw_start;
		while (y < draw_end)
		{
			mlx_put_pixel(game->canvas, x, y, color);
			y++;
		}
		x++;
	}
}

static void	render_player(t_game *game, t_map *scene, uint32_t color)
{
	t_v2i	start;
	t_v2i	end;

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
