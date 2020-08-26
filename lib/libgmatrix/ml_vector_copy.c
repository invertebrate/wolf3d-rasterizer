/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector_copy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 15:48:25 by veilo             #+#    #+#             */
/*   Updated: 2020/08/26 15:48:26 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

/*
**	Copies the elements of one vector to another for t_vec4
*/

void	ml_vector4_copy(t_vec4 src, t_vec4 dest)
{
	int i;

	i = 0;
	while (i < 4)
	{
		dest[i] = src[i];
		i++;
	}
}

/*
**	Copies the elements of one vector to another for t_vec3
*/

void	ml_vector3_copy(t_vec3 src, t_vec3 dest)
{
	int i;

	i = 0;
	while (i < 3)
	{
		dest[i] = src[i];
		i++;
	}
}
