#include "../cub3d.h"

int32_t	maxabs_int32(int32_t a, int32_t b)
{
	int32_t	a1;
	int32_t	b1;

	a1 = abs(a);
	b1 = abs(b);
	if (a1 > b1)
		return (a1);
	return (b1);
}

void	draw_line(mlx_image_t *grid, t_v2 v1, t_v2 v2, uint32_t color)
{
	t_v2	delta;
	t_v2	incr;
	int32_t	steps;
	int32_t	i;

	delta = vec_sub(v2, v1);
	steps = maxabs_int32(delta.x, delta.y);
	if (steps == 0)
	{
		mlx_put_pixel(grid, v1.x, v1.y, color);
		return ;
	}
	incr = vec_new(delta.x / steps, delta.y / steps);
	i = 0;
	while (i <= steps)
	{
		mlx_put_pixel(grid, v1.x, v1.y, color);
		v1.x += incr.x;
		v1.y += incr.y;
		i++;
	}
}
