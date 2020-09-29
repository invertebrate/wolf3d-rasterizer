/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:07:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/28 16:59:40 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB3D_H
# define LIB3D_H

# include "libgmatrix.h"
# include <stdarg.h>

# define EPSILON 0.00000001

typedef struct s_temp_calc
{
	t_vec3		normal;
	t_vec3		ax;
	t_vec3		cross_dir_ax;
	float		det;
	float		invdet;
}					t_temp_calc;

typedef struct		s_vertex
{
	t_vec4			position;
	t_vec3			normal;
	t_vec2			uv;
	unsigned int	color;
}					t_vertex;

/*
**	The box structures are rectangular and consist of points that for the
**	boundary. The top and bot part refer to the top and bottom rectangles
**	(or lines in 2d case). The rectangles are read counter-clock-wise order
**	starting from smallest coordinate. The a,b,c,d indices correspond to a,b,c,d
**	on the other side.
*/

typedef struct		s_box3d
{
	t_vec3			top_a;
	t_vec3			top_b;
	t_vec3			top_c;
	t_vec3			top_d;
	t_vec3			bot_a;
	t_vec3			bot_b;
	t_vec3			bot_c;
	t_vec3			bot_d;
}					t_box3d;

typedef struct		s_box2d
{
	t_vec2			top_a;
	t_vec2			top_b;
	t_vec2			bot_a;
	t_vec2			bot_b;
}					t_box2d;

/*
**	Triangle structure includes references to its vertices in an array for
**	Easier iteration. The vertex pointers directly point at the corresponding
**	vertices in the mesh vertex list.
*/

typedef struct		s_triangle
{
	t_vertex	*vtc[3];
	t_vec3		center; //consider if center calculated when mesh loaded
						//or center calculated when needed
	t_vec3		normal;
	t_vec3		ab;
	t_vec3		ac;
}					t_triangle;

typedef struct		s_ray
{
	t_vec3			origin;
	t_vec3			dir;
	t_vec3			normalized_dir;
}					t_ray;


typedef struct		s_intersection
{
	float			u;
	float			v;
	float			t;
	float			det;
}					t_intersection;

t_bool						triangle_intersection(t_triangle *triangle,
													t_ray *ray,
													t_intersection *is);
t_ray						new_ray(t_vec3 origin, t_vec3 direction);
void						l3d_triangle_normal_set(t_triangle *triangle);
void						l3d_triangle_centroid(t_triangle *triangle);

#endif
