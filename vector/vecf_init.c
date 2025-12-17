#include "../cub3d.h"

t_v2f	vecf_new(float_t x, float_t y)
{
	t_v2f	this;

	this.x = x;
	this.y = y;
	return (this);
}

t_v2f    vecf_from(t_v2f that)
{
	t_v2f	this;

	this.x = that.x;
	this.y = that.y;
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

t_v2f	vecf_rot(t_v2f that, float_t angle)
{
	float_t	cos_a;
	float_t	sin_a;
	t_v2f	this;

	cos_a = cosf(angle);
	sin_a = sinf(angle);
	this.x = that.x * cos_a - that.y * sin_a;
	this.y = that.x * sin_a + that.y * cos_a;
	return (this);
}
