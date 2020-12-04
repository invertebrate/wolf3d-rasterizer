/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 14:55:40 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/04 22:02:17 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Calculates & sets triangle's center. The center of mass to be more specific.
*/

void				l3d_triangle_centroid_update(t_triangle *triangle)
{
	ml_vector3_copy((t_vec3){
		(triangle->vtc[0]->pos[0] + triangle->vtc[1]->pos[0] +
		triangle->vtc[2]->pos[0]) / 3.0,
		(triangle->vtc[0]->pos[1] + triangle->vtc[1]->pos[1] +
		triangle->vtc[2]->pos[1]) / 3.0,
		(triangle->vtc[0]->pos[2] + triangle->vtc[1]->pos[2] +
		triangle->vtc[2]->pos[2]) / 3.0,
	}, triangle->center);
}

/*
** Out of all triangles inputted, calculates their center of mass.
*/

void				l3d_triangles_midpoint(t_triangle **triangles,
					uint32_t num_triangles, t_vec3 res)
{
	int		i;

	ft_memset(res, 0, sizeof(t_vec3));
	i = -1;
	while (++i < (int)num_triangles)
	{
		res[0] += triangles[i]->center[0] / num_triangles;
		res[1] += triangles[i]->center[1] / num_triangles;
		res[2] += triangles[i]->center[2] / num_triangles;
	}
}

/*
** Out of all triangles inputted (as a triangle vector), calculates their
** center of mass.
*/

void				l3d_triangle_vec_midpoint(t_tri_vec *triangles, t_vec3 res)
{
	int		i;

	ft_memset(res, 0, sizeof(t_vec3));
	i = -1;
	while (++i < (int)triangles->size)
	{
		res[0] += triangles->triangles[i]->center[0] / triangles->size;
		res[1] += triangles->triangles[i]->center[1] / triangles->size;
		res[2] += triangles->triangles[i]->center[2] / triangles->size;
	}
}

/*
** Sets triangle values
*/

void				l3d_triangle_set(t_triangle *triangle, t_vertex *vtc[3],
						t_3d_object *obj)
{
	triangle->material = obj->material;
	triangle->is_single_sided = L3D_SINGLE_SIDED;
	triangle->vtc[0] = vtc[0];
	triangle->vtc[1] = vtc[1];
	triangle->vtc[2] = vtc[2];
	l3d_triangle_centroid_update(triangle);
	l3d_triangle_normal_update(triangle);
}

t_triangle			*l3d_triangle_copy(t_triangle *src, t_bool new_vertices)
{
	t_triangle	*dst;
	int32_t		i;

	error_check(!(dst = malloc(sizeof(t_triangle))), "Failed to malloc tri");
	ft_memcpy(dst, src, sizeof(t_triangle));
	if (new_vertices)
	{
		i = -1;
		while (++i < 3)
		{
			error_check(!(dst->vtc[i] = malloc(sizeof(t_vertex))),
				"Failed to malloc vtc for tri");
			ft_memset(dst->vtc[i], 0, sizeof(t_vertex));
		}
	}
	return (dst);
}

void				l3d_triangle_destroy(t_triangle *triangle,
						t_bool with_vertices)
{
	int32_t	i;

	if (with_vertices)
	{
		i = -1;
		while (++i < 3)
		{
			free(triangle->vtc[i]);
			triangle->vtc[i] = NULL;
		}
	}
	free(triangle);
	triangle = NULL;
}
