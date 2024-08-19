/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wait.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 17:13:38 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 17:13:50 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int wait_threads(t_data *data)
{
	int	index;

	index = 0;
	while (index < data->nphilos)
	{
		if (pthread_join(data->philos[index]->thread, NULL) != 0)
			return (error(data, "phtread join error\n", 1));
		index++;
	}
	index = 0;
	while (index < data->nphilos)
	{
		if (pthread_mutex_destroy(&data->philos[index]->own) != 0)
			return (error(data, "philo mutex destroy error\n", 1));
		index++;
	}
	index = 0;
	while (index < data->nforks)
	{
		if (pthread_mutex_destroy(&data->forks[index]) != 0)
			return (error(data, "mutex destroy error\n", 1));
		index++;
	}
	if (pthread_mutex_destroy(&data->endmutex) != 0)
		return (error(data, "mutex destroy end error\n", 1));
	return (0);
}
