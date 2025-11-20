#include "../cub3d.h"

void	draw_line(mlx_image_t *grid, t_v2 v1, t_v2 v2, uint32_t color)
{
	double	delta_x;
	double	delta_y;
	double	steps;
	double	incr_x;
	double	incr_y;
	double	x;
	double	y;
	int32_t	i;

	delta_x = (double)(v2.x - v1.x);
	delta_y = (double)(v2.y - v1.y);
	steps = fmax(fabs(delta_x), fabs(delta_y));
	if (steps == 0)
	{
		mlx_put_pixel(grid, v1.x, v1.y, color);
		return ;
	}
	incr_x = delta_x / steps;
	incr_y = delta_y / steps;
	x = (double)v1.x;
	y = (double)v1.y;
	i = 0;
	while (i <= (int32_t)steps)
	{
		mlx_put_pixel(grid, round(x), round(y), color);
		x += incr_x;
		y += incr_y;
		i++;
	}
}
