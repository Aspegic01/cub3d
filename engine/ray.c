/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:22:56 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:22:59 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	ft_compute_ray_initial_length(t_ray *r, t_player *p, t_v2i *map)
{
	if (r->dir.x < 0)
	{
		r->step_dir.x = -1;
		r->length.x = (p->position.x - map->x) * r->step_size.x;
	}
	else
	{
		r->step_dir.x = 1;
		r->length.x = ((map->x + 1.0) - p->position.x) * r->step_size.x;
	}
	if (r->dir.y < 0)
	{
		r->step_dir.y = -1;
		r->length.y = (p->position.y - map->y) * r->step_size.y;
	}
	else
	{
		r->step_dir.y = 1;
		r->length.y = ((map->y + 1.0) - p->position.y) * r->step_size.y;
	}
}

void	ft_compute_ray_wall_distance(t_ray *ray, t_map *scene)
{
	t_v2i		map;
	uint32_t	hit;

	map = veci_fromf(scene->player.position);
	ft_compute_ray_initial_length(ray, &scene->player, &map);
	hit = 0;
	while (hit == 0)
	{
		if (ray->length.x < ray->length.y)
		{
			map.x += ray->step_dir.x;
			ray->distance = ray->length.x;
			ray->length.x += ray->step_size.x;
			ray->side = 0;
		}
		else
		{
			map.y += ray->step_dir.y;
			ray->distance = ray->length.y;
			ray->length.y += ray->step_size.y;
			ray->side = 1;
		}
		if (ft_at_wall(scene, map.x, map.y))
			hit = 1;
	}
}

t_ray	ft_init_ray(t_map *scene, float camerax)
{
	t_ray	ray;
	t_v2f	camera_offset;
	t_v2f	ray_offset;

	camera_offset = vecf_scale(scene->player.plane, camerax);
	ray.dir = vecf_add(scene->player.dir, camera_offset);
	ray.step_size = vecf_new(fabs(1 / ray.dir.x), fabs(1 / ray.dir.y));
	ray.step_dir = vecf_zero();
	ray.length = vecf_zero();
	ray.distance = 0.0;
	ray.side = 0;
	ft_compute_ray_wall_distance(&ray, scene);
	ray_offset = vecf_scale(ray.dir, ray.distance);
	ray.hit_pos = vecf_add(scene->player.position, ray_offset);
	return (ray);
}
