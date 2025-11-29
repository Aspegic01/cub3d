#include "../cub3d.h"

static int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	render_floor(t_game *game)
{
	uint32_t	boundary;
	uint32_t	x;
	uint32_t	y;
	uint32_t	c;

	y = 0;
	boundary = game->canvas->height / 2;
	while (y < boundary)
	{
		x = 0;
		while (x < game->canvas->width)
		{
			c = get_rgba(game->map->colors.ceiling_r,
					game->map->colors.ceiling_g, game->map->colors.ceiling_b,
					255);
			mlx_put_pixel(game->canvas, x, y, c);
			x++;
		}
		y++;
	}
}

void	render_ceiling(t_game *game)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	c;

	y = game->canvas->height / 2;
	while (y < game->canvas->height)
	{
		x = 0;
		while (x < game->canvas->width)
		{
			c = get_rgba(game->map->colors.floor_r, game->map->colors.floor_g,
					game->map->colors.floor_b, 255);
			mlx_put_pixel(game->canvas, x, y, c);
			x++;
		}
		y++;
	}
}

void	render_world(t_game *game)
{
	t_player *player;
	t_v2i	map;
	t_v2f	side_dist;
	t_v2f	delta;
	t_v2i	step;
	float_t	perp_wall_dist;
	int32_t hit;
	int32_t side;

	player = &game->map->player;
	map = veci_new(player->position.x / CELL_SIZE, player->position.y / CELL_SIZE);
	side_dist = vecf_zero();
	step = veci_zero();
	veci_print("map_position", map);
	vecf_print("postion", game->map->player.position);
	vecf_print("dirction", game->map->player.dir);
	vecf_print("plane", game->map->player.plane);

	uint32_t x = 0;
	while (x < (uint32_t)WIN_WIDTH)
	{
		float_t camerax = 2 * x / (float_t)WIN_WIDTH - 1;
		t_v2f ray_dir = vecf_add(player->dir, vecf_scale(player->plane, camerax));
		delta = vecf_new(fabs(1.0 / ray_dir.x), fabs(1.0 / ray_dir.y));
		if (ray_dir.x < 0)
		{
			step.x = -1;
			side_dist.x = (player->position.x - map.x) * delta.x;
		} else {
			step.x = 1;
			side_dist.x = (map.x + 1.0 - player->position.x) * delta.x;
		}
		if (ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = (player->position.y - map.y) * delta.y;
		} else {
			step.y = 1;
			side_dist.y = (map.y + 1.0 - player->position.y) * delta.y;
		}
		// DDA
		while (hit == 0)
		{
			//jump to next map square, either in x-direction, or in y-direction
			if (side_dist.x < side_dist.y)
			{
				side_dist.x += delta.x;
				map.x += step.x;
				side = 0;
			} else {
				side_dist.y += delta.y;
				map.y = step.y;
				side = 1;
			}
			if (map.x < (int32_t)ft_strlen(game->map->grid[map.x]) && game->map->grid[map.x][map.y] == 0)
				hit = 1;
		}
		//Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
		if (side == 0)
			perp_wall_dist = side_dist.x - delta.x;
		else
			perp_wall_dist = side_dist.y - delta.y;
		//Calculate height of line to draw on screen
		int32_t line_height = (int32_t)(WIN_HEIGHT / perp_wall_dist);
		//calculate lowest and highest pixel to fill in current stripe
		int32_t draw_start = -line_height / 2 + WIN_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		int32_t draw_end = line_height / 2 + WIN_HEIGHT / 2;
		if (draw_end >= WIN_HEIGHT)
			draw_end = WIN_HEIGHT - 1;
		
		uint32_t color = 0x00FF00FF;
		if (side == 1)
			color = color / 2;
		while (draw_start <= draw_end)
		{
			mlx_put_pixel(game->canvas, draw_start, draw_end, color);
			draw_start++;
		}
		x++;
	}
}

void	render_game(t_game *game)
{
	render_ceiling(game);
	render_floor(game);
	minimap_render(game->map);
	// render_world(game);
}
