#include "../cub3d.h"

t_v2	vec_new(int32_t x, int32_t y)
{
	t_v2	this;

	this.x = x;
	this.y = y;
	return (this);
}

t_v2	vec_zero(void)
{
	t_v2	this;

	this.x = 0;
	this.y = 0;
	return (this);
}

void	vec_print(char *label, t_v2 this)
{
	if (label && *label)
		printf("%s{x = %d, y = %d}\n", label, this.x, this.y);
	else
		printf("v{x = %d, y = %d}\n", this.x, this.y);
}
