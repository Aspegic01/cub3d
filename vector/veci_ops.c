/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   veci_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:05:05 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

t_v2i	veci_scale(t_v2i this, float_t factor)
{
	t_v2i	v;

	v.x = this.x * factor;
	v.y = this.y * factor;
	return (v);
}

t_v2i	veci_add(t_v2i this, t_v2i that)
{
	t_v2i	v;

	v.x = this.x + that.x;
	v.y = this.y + that.y;
	return (v);
}

t_v2i	veci_sub(t_v2i this, t_v2i that)
{
	t_v2i	v;

	v.x = this.x - that.x;
	v.y = this.y - that.y;
	return (v);
}

t_v2i	veci_div(t_v2i this, t_v2i that)
{
	t_v2i	v;

	v.x = this.x / that.x;
	v.y = this.y / that.y;
	return (v);
}

t_v2i	veci_mul(t_v2i this, t_v2i that)
{
	t_v2i	v;

	v.x = this.x * that.x;
	v.y = this.y * that.y;
	return (v);
}
