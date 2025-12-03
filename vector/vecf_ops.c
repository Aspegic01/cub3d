#include "../cub3d.h"

t_v2f	vecf_scale(t_v2f this, float_t factor)
{
	t_v2f	v;

	v.x = this.x * factor;
	v.y = this.y * factor;
	return (v);
}

t_v2f	vecf_add(t_v2f this, t_v2f that)
{
	t_v2f	v;

	v.x = this.x + that.x;
	v.y = this.y + that.y;
	return (v);
}

t_v2f	vecf_sub(t_v2f this, t_v2f that)
{
	t_v2f	v;

	v.x = this.x - that.x;
	v.y = this.y - that.y;
	return (v);
}

t_v2f	vecf_div(t_v2f this, t_v2f that)
{
	t_v2f	v;

	v.x = this.x / that.x;
	v.y = this.y / that.y;
	return (v);
}

t_v2f	vecf_mul(t_v2f this, t_v2f that)
{
	t_v2f	v;

	v.x = this.x * that.x;
	v.y = this.y * that.y;
	return (v);
}
