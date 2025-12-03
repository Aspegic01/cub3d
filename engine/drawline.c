#include "../cub3d.h"

void	draw_line(mlx_image_t *grid, t_v2f start, t_v2f end, uint32_t color)
{
	float	steps;
	t_v2f	delta;
	t_v2f	incr;
	int32_t	i;

	delta = vecf_sub(end, start);
	steps = fmax(fabs(delta.x), fabs(delta.y));
	if (steps == 0)
	{
		mlx_put_pixel(grid, start.x, start.y, color);
		return ;
	}
	incr = vecf_new(delta.x / steps, delta.y / steps);
	i = 0;
	while (i <= (int32_t)steps)
	{
		mlx_put_pixel(grid, round(start.x), round(start.y), color);
		start.x += incr.x;
		start.y += incr.y;
		i++;
	}
}
