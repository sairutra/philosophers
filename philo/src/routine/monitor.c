/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 17:10:31 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 18:06:38 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	thread_monitor_check_finish(t_data *data, int index)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&data->endmutex);
	pthread_mutex_lock(&data->philos[index]->own);
	if (data->philos[index]->status == finished)
		ret = 1;
	pthread_mutex_unlock(&data->philos[index]->own);
	pthread_mutex_unlock(&data->endmutex);
	return (ret);
}

int	thread_monitor_terminate_threads(t_data *data, int num)
{
	int			ret;
	long long	stamp;

	ret = 0;
	pthread_mutex_lock(&data->endmutex);
	pthread_mutex_lock(&data->philos[num]->own);
	stamp = timestamp();
	if ((stamp - data->philos[num]->tv) > data->die && data->philos[num]->tv)
	{
		printf("%lld %d died\n", (stamp - data->start), data->philos[num]->num);
		data->end = 1;
		ret = 1;
	}
	pthread_mutex_unlock(&data->philos[num]->own);
	pthread_mutex_unlock(&data->endmutex);
	return (ret);
}

void	thread_monitor(t_data *data)
{
	int	index;

	index = 0;
	while (1)
	{
		if (thread_monitor_terminate_threads(data, index))
			return ;
		if (thread_monitor_check_finish(data, index))
			return ;
		index++;
		if (index == data->nphilos)
			index = 0;
	}
}
