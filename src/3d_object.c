/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_object.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:22:11 by veilo             #+#    #+#             */
/*   Updated: 2020/09/28 17:31:42 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		obj_file_to_3d_obj(t_3d_object *obj, t_obj *read_obj)
{
	int		i;
	int		j;
	int		v_i;
	int		vt_i;
	// int		vn_i;

	i = -1;
	while (++i < (int)read_obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			v_i = read_obj->triangles[i * 9 + j * 3 + 0] - 1;
			vt_i = read_obj->triangles[i * 9 + j * 3 + 1] - 1;
			if (obj->vertices[v_i] == NULL)
				error_check(!(obj->vertices[v_i] =
					malloc(sizeof(t_vertex))), "Failed to malloc vertex");
			ml_vector3_copy(read_obj->v[v_i], obj->vertices[v_i]->position);
			obj->vertices[v_i]->color = 0xFFFFFFFF;
			ml_vector2_copy(read_obj->vt[vt_i], obj->vertices[v_i]->uv);
		}
		obj->triangles[i].vtc[0] =
			obj->vertices[read_obj->triangles[i * 9 + 0 * 3 + 0] - 1];
		obj->triangles[i].vtc[1] =
			obj->vertices[read_obj->triangles[i * 9 + 1 * 3 + 0] - 1];
		obj->triangles[i].vtc[2] =
			obj->vertices[read_obj->triangles[i * 9 + 2 * 3 + 0] - 1];
		l3d_triangle_centroid(&obj->triangles[i]);
	}
	free(read_obj->v);
	free(read_obj->vt);
	free(read_obj->vn);
	free(read_obj->triangles);
}

void		allocate_3d_object_content(t_3d_object *object, t_obj *read_obj)
{

	error_check(!(object->vertices =
		malloc(sizeof(t_vertex*) * read_obj->num_vertices)),
		"Failed to malloc 3d obj vertices");
	ft_memset(object->vertices, 0,
		sizeof(t_vertex*) *  read_obj->num_vertices);
	error_check(!(object->triangles =
		malloc(sizeof(t_triangle) *  read_obj->num_triangles)),
		"Failed to malloc 3d obj triangles");
	error_check(!(object->uvs =
		malloc(sizeof(t_vec2) *  read_obj->num_v_text_coords)),
		"Failed to malloc 3d obj uvs");
}

void		obj_content_to_scene_data(t_scene_data *data, t_obj_content *obj)
{
	int			num_triangles;
	int			i;
	int			j;

	error_check(!(data->objects =
		malloc(sizeof(*data->objects) * obj->num_objects)),
		"Failed to malloc 3d obj");
	i = -1;
	num_triangles = 0;
	while (++i < (int)obj->num_objects)
	{
		error_check(!(data->objects[i] = malloc(sizeof(t_3d_object))),
		"Failed to malloc 3d obj");
		allocate_3d_object_content(data->objects[i], &obj->objects[i]);
		obj_file_to_3d_obj(data->objects[i], &obj->objects[i]);
		data->objects[i]->num_triangles = obj->objects[i].num_triangles;
		data->objects[i]->num_vertices = obj->objects[i].num_vertices;
		num_triangles -= 1;
		j = 0;
		while (++num_triangles < (int)data->num_triangles + data->objects[i]->num_triangles)
			data->triangle_ref[num_triangles] = &data->objects[i]->triangles[j++];
		data->num_triangles += data->objects[i]->num_triangles;
	}
}

void		transform_3d_object(t_3d_object *obj, t_mat4 transform)
{
	int		i;
	int		j;

	i = -1;
	while (++i < obj->num_vertices)
		ml_matrix4_mul_vec3(transform,
			obj->vertices[i]->position, obj->vertices[i]->position);
	j = -1;
	while (++j < obj->num_triangles)
		l3d_triangle_normal_set(&obj->triangles[j]);
}


void		destroy_object(t_3d_object *object)
{
	int		i;

	free(object->triangles);
	i = -1;
	while (++i < object->num_vertices)
		free(object->vertices[i]);
	free(object->vertices);
	free(object);
	object = NULL;
}
