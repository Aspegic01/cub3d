/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   veci_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:05:05 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

t_v2i	veci_new(int32_t x, int32_t y)
{
	t_v2i	this;

	this.x = x;
	this.y = y;
	return (this);
}

t_v2i	veci_from(t_v2i that)
{
	t_v2i	this;

	this.x = that.x;
	this.y = that.y;
	return (this);
}

t_v2i	veci_fromf(t_v2f that)
{
	t_v2i	this;

	this.x = that.x;
	this.y = that.y;
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
