#include "../cub3d.h"

t_v2	vec_scale(t_v2 this, float factor)
{
	t_v2	v;

	v.x = this.x * factor;
	v.y = this.y * factor;
	return (v);
}

t_v2	vec_add(t_v2 this, t_v2 that)
{
	t_v2	v;

	v.x = this.x + that.x;
	v.y = this.y + that.y;
	return (v);
}

t_v2	vec_sub(t_v2 this, t_v2 that)
{
	t_v2	v;

	v.x = this.x - that.x;
	v.y = this.y - that.y;
	return (v);
}

t_v2	vec_div(t_v2 this, t_v2 that)
{
	t_v2	v;

	if (that.x == 0 || that.y == 0)
		return (vec_zero());
	v.x = this.x / that.x;
	v.y = this.y / that.y;
	return (v);
}

t_v2	vec_mul(t_v2 this, t_v2 that)
{
	t_v2	v;

	v.x = this.x * that.x;
	v.y = this.y * that.y;
	return (v);
}
