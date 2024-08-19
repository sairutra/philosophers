/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 17:06:52 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 18:10:10 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	thread_init_philo_assign_forks(t_data *data, t_philo *philo, int index)
{
	if (index == 0)
	{
		philo->left = &data->forks[0];
		philo->right = &data->forks[data->nphilos - 1];
	}
	else if (data->nforks == 2)
	{
		philo->left = &data->forks[0];
		philo->right = &data->forks[1];
	}
	else
	{
		philo->left = &data->forks[index];
		philo->right = &data->forks[index - 1];
	}
}

int	thread_init_philo(t_data *data, int index)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	memset(philo, 0, sizeof(t_philo));
	if (philo == NULL)
		return (error(data, "threads malloc error", 1));
	philo->main = data;
	philo->start = philo->main->start;
	philo->num = index + 1;
	if (data->lunches > 0)
		philo->max_lunch = data->lunches;
	thread_init_philo_assign_forks(data, philo, index);
	if (pthread_mutex_init(&philo->own, NULL) != 0)
		return (error(data, "philo mutex error\n", 1));
	data->philos[index] = philo;
	if (pthread_create(&data->philos[index]->thread, NULL, \
	&routine, (void *)data->philos[index]) != 0)
		return (error(data, "phtread create error\n", 1));
	return (0);
}

int	thread_init(t_data *data)
{
	int	index;
	int	size;

	size = data->nphilos * sizeof(t_philo **);
	index = 0;
	data->philos = malloc(size);
	memset(data->philos, 0, size);
	if (data->philos == NULL)
		return (error(data, "threads malloc error", 1));
	pthread_mutex_lock(&data->endmutex);
	data->start = timestamp();
	while (index < data->nphilos)
	{
		if (thread_init_philo(data, index))
			return (1);
		index++;
	}
	pthread_mutex_unlock(&data->endmutex);
	thread_monitor(data);
	return (0);
}
