#include "../cub3d.h"

t_v2i	veci_new(int32_t x, int32_t y)
{
	t_v2i	this;

	this.x = x;
	this.y = y;
	return (this);
}

t_v2i	veci_zero(void)
{
	t_v2i	this;

	this.x = 0;
	this.y = 0;
	return (this);
}

void	veci_print(char *label, t_v2i this)
{
	if (label && *label)
		printf("%s{x = %d, y = %d}\n", label, this.x, this.y);
	else
		printf("v{x = %d, y = %d}\n", this.x, this.y);
}


