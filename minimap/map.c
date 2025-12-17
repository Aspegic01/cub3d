#include "../cub3d.h"
#include <stdint.h>

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

	scene->tex_north = mlx_load_png(scene->textures.north);
	scene->tex_south = mlx_load_png(scene->textures.south);
	scene->tex_west  = mlx_load_png(scene->textures.west);
	scene->tex_east  = mlx_load_png(scene->textures.east);
	if (!scene->tex_north || !scene->tex_south ||
		!scene->tex_west || !scene->tex_east)
	{
		exit(1);
	}
	

}

bool	at_wall(t_map *map, float_t offsetx, float_t offsety)
{
	int32_t	x;
	int32_t	y;

	x = (int)offsetx;
	y = (int)offsety;
	if (y <= 0 || y >= map->height || x <= 0 || x >= map->width)
		return (true);
	if (x >= (int32_t)ft_strlen(map->grid[y]))
		return (true);
	return (map->grid[y][x] != '0');
}

static uint32_t	get_pixel_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return ((uint32_t)r << 24 | (uint32_t)g << 16 | (uint32_t)b << 8 | a);
}

static uint32_t	darken_color(uint32_t color)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	r = (color >> 24) & 0xFF;
	g = (color >> 16) & 0xFF;
	b = (color >> 8) & 0xFF;
	a = color & 0xFF;
	return (get_pixel_color(r / 2, g / 2, b / 2, a));
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

void	draw_fov(t_game *game, t_map *scene, t_player *player, uint32_t color)
{
	uint32_t x = 0;
	while (x < game->canvas->width)
	{
		float_t camerax = 2.0 * (x / (float_t)game->canvas->width) - 1.0;
		uint32_t hit = 0;
		uint32_t side = 0;
		float_t distance = 0;
		t_v2i map = veci_new(player->position.x, player->position.y);
		t_v2f step = vecf_zero();
		t_v2f ray_dir = vecf_add(player->dir, vecf_scale(player->plane, camerax));
		t_v2f rayLenght1D = vecf_zero();
		t_v2f rayStepSize = vecf_zero();

		if (ray_dir.x == 0.0)
			rayStepSize.x = 1e30;
		else
			rayStepSize.x = fabs(1 / ray_dir.x);
		if (ray_dir.y == 0)
			rayStepSize.y = 1e30;
		else
			rayStepSize.y = fabs(1 / ray_dir.y);

		if (ray_dir.x < 0)
		{
			step.x = -1;
			rayLenght1D.x = (player->position.x - map.x) * rayStepSize.x;
		} else {
			step.x = 1;
			rayLenght1D.x = ((map.x + 1.0) - player->position.x) * rayStepSize.x;
		}
		if (ray_dir.y < 0)
		{
			step.y = -1;
			rayLenght1D.y = (player->position.y - map.y) * rayStepSize.y;
		} else {
			step.y = 1;
			rayLenght1D.y = ((map.y + 1.0) - player->position.y) * rayStepSize.y;
		}
		while (hit == 0)
		{
			if (rayLenght1D.x < rayLenght1D.y)
			{
				map.x += step.x;
				distance = rayLenght1D.x;
				rayLenght1D.x += rayStepSize.x;
				side = 0;
			} else {
				map.y += step.y;
				distance = rayLenght1D.y;
				rayLenght1D.y += rayStepSize.y;
				side = 1;
			}
			if (at_wall(scene, map.x, map.y))
				hit = 1;
		}
		
		// t_v2f end = vecf_add(player->position, vecf_scale(ray_dir, distance));
		// draw_line(game->map->img, vecf_scale(player->position, CELL_SIZE), vecf_scale(end, CELL_SIZE), color);
		int32_t line_height;
		if (distance == 0)
			line_height = (int32_t)game->canvas->height / 1;
		else
			line_height = (int32_t)game->canvas->height / distance;
		int32_t draw_start = -line_height / 2 + game->canvas->height / 2;
		if (draw_start < 0)
			draw_start = 0;
		int32_t draw_end = line_height / 2 + game->canvas->height / 2;
		if (draw_end >= (int32_t)game->canvas->height)
			draw_end = game->canvas->height - 1;
		int32_t y = draw_start;
		/* choose texture based on side and step direction */
		mlx_texture_t *tex = NULL;
		// scene->tex_north = mlx_load_png(scene->textures.north);
		// scene->tex_south = mlx_load_png(scene->textures.south);
		// scene->tex_west  = mlx_load_png(scene->textures.west);
		// scene->tex_east  = mlx_load_png(scene->textures.east);
		//
		// if (!scene->tex_north || !scene->tex_south ||
		// 	!scene->tex_west || !scene->tex_east)
		// {
		// 	ft_putstr_fd("Texture load error\n", 2);
		// 	exit(1);
		// }

		if (side == 0)
		{
			if (step.x < 0)
				tex = scene->tex_west;
			else
				tex = scene->tex_east;
		}
		else
	{
			if (step.y < 0)
				tex = scene->tex_north;
			else
				tex = scene->tex_south;
		}
		if (tex)
		{
			/* compute exact hit position on the wall */
			t_v2f hit_pos = vecf_add(player->position, vecf_scale(ray_dir, distance));
			float wallX;
			if (side == 0)
				wallX = hit_pos.y;
			 else
				wallX = hit_pos.x;
			wallX -= floor(wallX);
			int texX = (int)(wallX * (float)tex->width);
			if (texX < 0)
				texX = 0;
			if (texX >= (int)tex->width)
				texX = tex->width - 1;

			float texStep = (float)tex->height / (float)line_height;
			float texPos = (draw_start - (float)game->canvas->height / 2 + (float)line_height / 2) * texStep;
			while (y < draw_end)
			{
				int texY = (int)texPos;
				if (texY < 0)
					texY = 0;
				if (texY >= (int)tex->height)
					texY = tex->height - 1;
				int idx = (texY * tex->width + texX) * tex->bytes_per_pixel;
				uint8_t *p = &tex->pixels[idx];
				uint32_t pixcol = get_pixel_color(p[0], p[1], p[2], p[3]);
				if (side == 1)
					pixcol = darken_color(pixcol);
				mlx_put_pixel(game->canvas, x, y, pixcol);
				texPos += texStep;
				y++;
			}
		}
		else
		{
			int wall_color = color;
			if (side == 1)
				wall_color = 0x0000ff00;
			while (y < draw_end)
				mlx_put_pixel(game->canvas, x, ++y, wall_color);
		}
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
	draw_fov(game, scene, &scene->player, 0xff00ffff);
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
