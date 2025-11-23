#include "../cub3d.h"

t_v2f	vecf_new(float_t x, float_t y)
{
	t_v2f	this;

	this.x = x;
	this.y = y;
	return (this);
}

t_v2f	vecf_zero(void)
{
	t_v2f	this;

	this.x = 0;
	this.y = 0;
	return (this);
}

void	vecf_print(char *label, t_v2f this)
{
	if (label && *label)
		printf("%s{x = %f, y = %f}\n", label, this.x, this.y);
	else
		printf("v{x = %f, y = %f}\n", this.x, this.y);
}
