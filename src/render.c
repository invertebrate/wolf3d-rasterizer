/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/30 18:07:34 by veilo             #+#    #+#             */
/*   Updated: 2020/08/30 18:07:37 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//SERIOUSLY consider if vertices should be in contiguous array
//and triangles just indexing them
//that would make looping vertices much easier

//add baryocentric coordinatesystem to triangle for easier
//fragment shader use


#include "wolf3d.h"

// t_mesh		*create_renderable_mesh(t_mesh *mesh, t_shader *shader,
// 									int *framebuffer)
// //framebuffer input here questionable?
// {
// 	t_mesh *mesh;

// 	return (mesh);
// }

t_bool		render_triangle(t_triangle *triangle, t_mesh *mesh, t_camera *camera)
{
	int is;

	is = 0;
	t_intersection		intsec;
	int					color;
	int					i;
	(void)mesh;
	color = 0xffffffff;
	i = -1;
	// ft_printf("count: %d\nwidth: %d\nheight: %d\n", camera->raycount,
	// 			(int)camera->screen_width, (int)camera->screen_height);
	//project mesh bounding box on screen
	//decide later if bounding boxes for each triangle are needed for performance
	while (++i < camera->raycount)
	{
		if (triangle_intersection(triangle, camera->rays[i], &intsec))
		{
			camera->framebuffer[screen_to_frame_coords(camera->parent_scene,
				-1 * (camera->rays[i].dir[1]) + camera->screen_width / 2,
				-1 * (camera->rays[i].dir[2]) + camera->screen_height / 2)] = color;
			is++;
			// ft_printf("is: %d\n", is);
			// ft_printf("x: %d\n", -1 * camera->rays[i].dir[1]);
			// ft_printf("y: %d\n", -1 * camera->rays[i].dir[2]);
			// ft_printf("x: %d\n", -1 * (int)camera->rays[i].dir[1] + WIDTH / 2);
			// ft_printf("y: %d\n", -1 * (int)camera->rays[i].dir[2] + HEIGHT / 2);
		}
		//find a way to get uv data in fragment shader
		//mesh->shader->f(triangle, calculate_baryocoords(intsec), color);
		//find a way to get framebuffer data in and or out of this function

	}
	is = 0;
	return (true);
}

// t_bool		render_mesh(t_mesh *mesh, t_camera *camera)
// {
// 	//for (vertex in triangle in mesh)
// 	//{
// 		//can't actually modify original vertex data
// 		//figure out if create temporary copy of all vertices
// 		//or some other way
// 	((*mesh).shader)->v(vertex, mesh, camera);
// 	//}
// 	while (triangles)
// 	{
// 		render_triangle(triangles[i], mesh, camera);
// 	}
// 	return (true);
// }
