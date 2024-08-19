/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 17:03:06 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 18:11:46 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	data_init_var(t_data *data, int *data_var, char *var, char *err_msg)
{
	*data_var = philo_atoi(var);
	if (*data_var == -1)
		return (error(data, err_msg, 1));
	return (0);
}

int	data_init(t_data *data, char **argv)
{
	memset(data, 0, sizeof(t_data));
	if (data_init_var(data, &data->nphilos, argv[1], "num of philos wrong\n"))
		return (1);
	data->nforks = data->nphilos;
	if (data_init_var(data, &data->die, argv[2], "time to die wrong\n"))
		return (1);
	if (data_init_var(data, &data->eat, argv[3], "time to eat wrong\n"))
		return (1);
	if (data_init_var(data, &data->sleep, argv[4], "time to sleep wrong\n"))
		return (1);
	if (argv[5])
	{
		if (data_init_var(data, &data->lunches, argv[5], "lunches wrong\n"))
			return (1);
	}
	if (data->nphilos > 200)
		return (error(data, "too many philos, not more than 200 allowed\n", 1));
	if (data->die < 60)
		return (error(data, "time to die too low, not lower than 60\n", 1));
	if (data->eat < 60)
		return (error(data, "time to eat too low, not lower than 60\n", 1));
	if (data->sleep < 60)
		return (error(data, "time to sleep too low, not lower than 60\n", 1));
	return (0);
}

int	mutex_init(t_data *data)
{
	int	index;
	int	size;

	size = data->nforks * sizeof(pthread_mutex_t);
	index = 0;
	data->forks = malloc(size);
	memset(data->forks, 0, size);
	if (data->forks == NULL)
		return (error(data, "threads malloc error", 1));
	while (index < data->nforks)
	{
		if (pthread_mutex_init(&data->forks[index], NULL) != 0)
			return (error(data, "mutex init error\n", 1));
		index++;
	}
	if (pthread_mutex_init(&data->endmutex, NULL) != 0)
		return (error(data, "mutex init end error\n", 1));
	return (0);
}
