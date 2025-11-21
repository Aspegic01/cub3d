#include "../cub3d.h"

void	draw_line(mlx_image_t *grid, double v1_x, double v1_y, double v2_x, double v2_y, uint32_t color)
{
	double	delta_x;
	double	delta_y;
	double	steps;
	double	incr_x;
	double	incr_y;
	double	x;
	double	y;
	int32_t	i;

	delta_x = (double)(v2_x - v1_x);
	delta_y = (double)(v2_y - v1_y);
	steps = fmax(fabs(delta_x), fabs(delta_y));
	if (steps == 0)
	{
		mlx_put_pixel(grid, v1_x, v1_y, color);
		return ;
	}
	incr_x = delta_x / steps;
	incr_y = delta_y / steps;
	x = v1_x;
	y = v1_y;
	i = 0;
	while (i <= (int32_t)steps)
	{
		mlx_put_pixel(grid, round(x), round(y), color);
		x += incr_x;
		y += incr_y;
		i++;
	}
}
