/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:04:28 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/14 13:18:57 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	philo_atoi(char *arg)
{
	int	result;
	int	index;

	index = 0;
	result = 0;
	while (arg[index] != 0)
	{
		if (!((arg[index] > 47) && (arg[index] < 58)))
			return (-1);
		result = result * 10 + (arg[index] - 48);
		index++;
	}
	return (result);
}


void print_err(char * msg)
{
	int index;

	index = 0;
	while (msg[index] != '\0')
		index++;
	write(2, msg, index);
}

void print_out(char * msg)
{
	int index;

	index = 0;
	while (msg[index] != '\0')
		index++;
	write(1, msg, index);
}


void free_all(t_data *data)
{
	int	index;
	
	index = 0;
	if (data->philos)
	{
		while (index < data->nforks)
			free(data->philos[index++]);
	}
	free(data->philos);
	free(data->forks);
}


int error(t_data *data, char *msg, int exitcode)
{
	print_err(msg);
	free_all(data);
	return (exitcode);
}


int	philos_init_var(t_data *data, int* data_var, char *var, char *err_msg)
{
	*data_var = philo_atoi(var);
	if (*data_var == -1)
		return (error(data, err_msg, 1));
	return (0);
}

void philos_init_seconds(t_data *data)
{
	data->die = data->die * 1000;
	data->sleep = data->sleep * 1000;
	data->eat = data->eat * 1000;
}

int	philos_init(t_data *data, char **argv)
{
	memset(data, 0, sizeof(t_data));
	if (philos_init_var(data, &data->nphilos, argv[1], "number of philos wrong\n"))
		return (1);
	data->nforks = data->nphilos;
	if (philos_init_var(data, &data->die, argv[2], "time to die wrong\n"))
			return (1);
	if (philos_init_var(data, &data->eat, argv[3], "time to eat wrong\n"))
			return (1);
	if (philos_init_var(data, &data->sleep, argv[4], "time to sleep wrong\n"))
			return (1);
	if (argv[5])
	{
		if (philos_init_var(data, &data->lunches, argv[5], "amount of lunches wrong\n"))
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
	philos_init_seconds(data);
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
	return (0);
}

void routine_print(t_philo *philo, int status)
{
	struct timeval tv;

	philo->status = status;
	gettimeofday(&tv, NULL);
	if (status == eating && !philo->main->end)
	{
		printf("%ld %d is eating\n", tv.tv_usec, philo->num);
		usleep(philo->main->eat);
		philo->lunches++;
		if (philo->lunches == philo->max_lunch)
			philo->status = finished;
	}
	if (status == thinking && !philo->main->end)
		printf("%ld %d is thinking\n", tv.tv_usec, philo->num);
	if (status == sleeping && !philo->main->end)
	{
		printf("%ld %d is sleeping\n", tv.tv_usec, philo->num);
		usleep(philo->main->sleep);
	}
}

void routine_lock(t_philo *philo)
{
	struct timeval tv;

	pthread_mutex_lock(philo->left);
	gettimeofday(&tv, NULL);
	printf("%ld %d has taken a fork\n", tv.tv_usec, philo->num);
	pthread_mutex_lock(philo->right);
	gettimeofday(&tv, NULL);
	printf("%ld %d has taken a fork\n", tv.tv_usec, philo->num);
}

void routine_unlock(t_philo *philo)
{
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}

void *routine(void *arg)
{
	t_philo *philo;
	struct timeval tv;

	philo = (t_philo*)arg;
	gettimeofday(&philo->tv, NULL);
	while (1)
	{
		gettimeofday(&tv, NULL);
		if ((tv.tv_usec - philo->tv.tv_usec) > philo->main->die)
		{
			philo->status = death;
			return (NULL);
		}
		else
			philo->tv = tv;
		routine_lock(philo);
		routine_print(philo, eating);
		routine_unlock(philo);
		if (philo->lunches == philo->max_lunch || philo->main->end)
			return (NULL);
		routine_print(philo, sleeping);
		routine_print(philo, thinking);
	}
	return (NULL);
}

int	thread_init_philo(t_data *data, int index)
{
	t_philo *philo;

	philo = malloc(sizeof(t_philo));
	memset(philo, 0, sizeof(t_philo));
	if (philo == NULL)
		return (error(data, "threads malloc error", 1));
	philo->main = data;
	philo->num = index + 1;
	if (data->lunches > 0)
		philo->max_lunch = data->lunches;
	if (index == 0)
	{
		philo->left = &data->forks[0];
		philo->right = &data->forks[data->nforks - 1];
	}
	else
	{
		philo->left = &data->forks[index];
		philo->right = &data->forks[index - 1];
	}
	data->philos[index] = philo;
	if (pthread_create(&data->philos[index]->thread, NULL, &routine, (void *)data->philos[index]) != 0)
		return (error(data, "phtread create error\n", 1));
	return (0);
}

int	thread_monitor_check_finish(t_data *data)
{
	int	index;

	index = 0;
	while (index < data->nphilos)
	{
		if (data->philos[index]->status != finished)
			return (0);
		index++;
	}
	return (1);
}


void thread_monitor(t_data *data)
{
	int	index;
	struct timeval tv;

	index = 0;
	while (1)
	{
		if (data->philos[index]->status == death)
		{
			gettimeofday(&tv, NULL);
			printf("%ld %d died\n", tv.tv_usec, data->philos[index]->num);
			data->end = 1;
			return ;
		}
		if (data->philos[index]->status == finished)
		{
			if (thread_monitor_check_finish(data))
				return ;
		}
		index++;
		if (index == data->nphilos)
			index = 0;
	}
}


int thread_init(t_data *data)
{
	int	index;
	int	size;

	size = data->nphilos * sizeof(t_philo**);
	index = 0;
	data->philos = malloc(size);
	memset(data->philos, 0, size);
	if (data->philos == NULL)
		return (error(data, "threads malloc error", 1));
	while (index < data->nphilos)
	{
		if (thread_init_philo(data, index))
			return (1);
		index++;
	}
	thread_monitor(data);
	return (0);
}

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
	while (index < data->nforks)
	{
		if (pthread_mutex_destroy(&data->forks[index]) != 0)
			return (error(data, "mutex destroy error\n", 1));
		index++;
	}
	return (0);
}


int main(int argc, char **argv)
{
	t_data data;

	if (argc < 5 || argc > 6)
		return (error(&data, "wrong amount of arguments\n", 1));
	if (philos_init(&data, argv))
		return (1);
	if (mutex_init(&data))
		return (1);
	if (thread_init(&data))
		return (1);
	if (wait_threads(&data))
		return (1);
	free_all(&data);
	return (0);
}
