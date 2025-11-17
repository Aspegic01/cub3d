#include "../cub3d.h"

static int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void	render_game(t_game *game)
{
	uint32_t	x;
	uint32_t	y;
	t_colors	colors;
	int	color;

	colors = game->map->colors;
	y = 0;
	while (y < game->canvas->height)
	{
		x = 0;
		while (x < game->canvas->width)
		{
			color = get_rgba(colors.ceiling_r, colors.ceiling_g, colors.ceiling_b, 255);
			mlx_put_pixel(game->canvas, x, y, color);
			x++;
		}
		y++;
	}

	t_v2 pos = vec_new(game->canvas->width / 2 - game->map->cell_size, game->canvas->height - game->map->cell_size);
	y = 0;
	while (y < (uint32_t)game->map->cell_size)
	{
		x = 0;
		while (x < (uint32_t)game->map->cell_size)
		{
			mlx_put_pixel(game->canvas, x + pos.x, y + pos.y, 0x0000FFFF);
			x++;
		}
		y++;
	}
}
