/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:58:24 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 02:06:31 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"

t_test_state	test_state_create(t_bool success, const char *result, int id)
{
	t_test_state	state;

	state = (t_test_state){.success = success, .result = result, .id = id};
	return (state);
}

void			update_test_state(const char *(*test)(void))
{
	const char		*message;
	t_test_state	state;

	message = test();
	g_tests_run++;
	state = test_state_create(message ? false : true,
		message, g_tests_run - 1);
	g_test_states[g_tests_run - 1] = state;
	ft_printf(" test: %d: %s\n", state.id,
		state.success ? "Success" : state.result);
}
