/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:04:28 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/13 16:53:34 by spenning      ########   odam.nl         */
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


void free_all(t_philos *data)
{
	free(data->thr);
	free(data->forks);
}


int error(t_philos *data, char *msg, int exitcode)
{
	print_err(msg);
	free_all(data);
	return (exitcode);
}


int	philos_init_var(t_philos *data, int* data_var, char *var, char *err_msg)
{
	*data_var = philo_atoi(var);
	if (*data_var == -1)
		return (error(data, err_msg, 1));
	return (0);
}

int	philos_init(t_philos *data, char **argv)
{
	memset(data, 0, sizeof(t_philos));
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
	if (data->nphilos >= 200)
		return (error(data, "too many philos, not more than 200 allowed\n", 1));
	if (data->die <= 60)
		return (error(data, "time to die too low, not lower than 60\n", 1));
	if (data->eat <= 60)
		return (error(data, "time to eat too low, not lower than 60\n", 1));
	if (data->sleep <= 60)
		return (error(data, "time to sleep too low, not lower than 60\n", 1));
	return (0);
}

void *routine(void *arg)
{
	(void)arg;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	printf("%ld\n", tv.tv_sec);
	return (NULL);
}


int thread_init(t_philos *data)
{
	int	index;
	int	size;

	size = data->nphilos * sizeof(pthread_t);
	index = 0;
	data->thr = malloc(size);
	memset(data->thr, 0, size);
	if (data->thr == NULL)
		return (error(data, "threads malloc error", 1));
	while (index < data->nphilos)
	{
		if (pthread_create(&data->thr[index], NULL, &routine, NULL) != 0)
			return (error(data, "phtread create error\n", 1));
		index++;
	}
	return (0);
}

int	mutex_init(t_philos *data)
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

int wait_threads(t_philos *data)
{
	int	index;

	index = 0;
	while (index < data->nphilos)
	{
		if (pthread_join(data->thr[index], NULL) != 0)
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
	t_philos data;

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
