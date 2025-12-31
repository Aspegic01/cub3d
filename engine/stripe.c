/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:22:56 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:22:59 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static inline int32_t	ft_min(int32_t a, int32_t b)
{
	if (a < b)
		return (a);
	return (b);
}

static inline int32_t	ft_max(int32_t a, int32_t b)
{
	if (a > b)
		return (a);
	return (b);
}

t_wall_stripe	ft_compute_wall_stripe(t_map *scene, float_t camerax)
{
	t_wall_stripe	stripe;
	int32_t			sheight;

	stripe.ray = ft_init_ray(scene, camerax);
	sheight = ft_get_lineheight(stripe.ray.distance);
	stripe.height = sheight;
	stripe.start = ft_max(-sheight / 2 + WIN_HEIGHT / 2, 0);
	stripe.end = ft_min(stripe.start + sheight - 1, WIN_HEIGHT - 1);
	return (stripe);
}
