/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vecf_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:05:05 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

t_v2f	vecf_scale(t_v2f this, float factor)
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
