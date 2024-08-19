/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wait.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 17:13:38 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 18:00:07 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	wait_thread_join(t_data *data)
{
	int	index;

	index = 0;
	while (index < data->nphilos)
	{
		if (pthread_join(data->philos[index]->thread, NULL) != 0)
			return (error(data, "phtread join error\n", 1));
		index++;
	}
	return (0);
}

int	wait_thread_destroy_philo_mutex(t_data *data)
{
	int	index;

	index = 0;
	while (index < data->nphilos)
	{
		if (pthread_mutex_destroy(&data->philos[index]->own) != 0)
			return (error(data, "philo mutex destroy error\n", 1));
		index++;
	}
	return (0);
}

int	wait_thread_destroy_forks(t_data *data)
{
	int	index;

	index = 0;
	while (index < data->nforks)
	{
		if (pthread_mutex_destroy(&data->forks[index]) != 0)
			return (error(data, "mutex destroy error\n", 1));
		index++;
	}
	return (0);
}

int	wait_thread_destroy_mutexes(t_data *data)
{
	if (wait_thread_destroy_philo_mutex(data))
		return (1);
	if (wait_thread_destroy_forks(data))
		return (1);
	if (pthread_mutex_destroy(&data->endmutex) != 0)
		return (error(data, "mutex destroy end error\n", 1));
	return (0);
}

int	wait_threads(t_data *data)
{
	int	index;

	if (wait_thread_join(data))
		return (1);
	if (wait_thread_destroy_mutexes(data))
		return (1);
	return (0);
}
