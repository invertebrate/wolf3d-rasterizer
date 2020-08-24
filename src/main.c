/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 14:59:06 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/24 15:39:34 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int main(void)
{
	t_wolf3d	app;
	
	wolf3d_run(&app);
	return (EXIT_SUCCESS);
}