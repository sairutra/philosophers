/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:04:28 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/15 18:53:23 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	philo_len(const char *arg)
{
	int	index;

	index = 0;
	while (arg[index] != '\0')
		index++;
	return (index);
}

int	philo_strncmp(const char *s1, const char *s2, size_t n)
{
	int		retvalue;
	size_t	index;
	size_t	s1_len;
	size_t	s2_len;

	retvalue = 0;
	index = 0;
	s1_len = philo_len(s1);
	s2_len = philo_len(s2);
	while (index < n && (index < s1_len || index < s2_len))
	{
		retvalue += ((unsigned char)s1[index] - (unsigned char)s2[index]);
		index++;
	}
	return (retvalue);
}
void print_err(char * msg)
{
	int index;

	index = 0;
	while (msg[index] != '\0')
		index++;
	write(2, msg, index);
}

int	philo_atoi(char *arg)
{
	int	result;
	int	index;

	index = 0;
	result = 0;
	if (philo_len(arg) > 10)
	{
		print_err("integer overflow: ");
		return (-1);
	}
	if (philo_strncmp(arg, "2147483647", 10) > 0 && philo_len(arg) == 10)
	{
		print_err("integer overflow: ");
		return (-1);
	}
	while (arg[index] != 0)
	{
		if (!((arg[index] > 47) && (arg[index] < 58)))
			return (-1);
		result = result * 10 + (arg[index] - 48);
		index++;
	}
	return (result);
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
	(void)data;
	// data->die = data->die * 1000;
	// data->sleep = data->sleep * 1000;
	// data->eat = data->eat * 1000;
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
	if (pthread_mutex_init(&data->endmutex, NULL) != 0)
		return (error(data, "mutex init end error\n", 1));
	return (0);
}

long long timestamp(void)
{
	long long ret;
	struct timeval stamp;

	gettimeofday(&stamp, NULL);
	ret = (stamp.tv_sec * 1000) + (stamp.tv_usec / 1000);
	return (ret);
}

void philo_usleep(int mili)
{
	long long wakeup;

	wakeup = mili + timestamp();
	while (wakeup > timestamp())
		usleep(100);
	return ;
}

void routine_print(t_philo *philo, int status)
{
	pthread_mutex_lock(&philo->main->endmutex);
	philo->status = status;
	if (status == eating && philo->main->end == 0)
	{
		philo->tv = timestamp();
		printf("%lld %d is eating\n", (philo->tv - philo->main->start), philo->num);
		philo->lunches++;
		if (philo->lunches == philo->max_lunch)
			philo->status = finished;
	}
	if (status == idle && philo->main->end == 0)
		printf("%lld %d has taken a fork\n", (timestamp() - philo->main->start), philo->num);
	if (status == thinking && philo->main->end == 0)
		printf("%lld %d is thinking\n", (timestamp() - philo->main->start), philo->num);
	if (status == sleeping && philo->main->end == 0)
		printf("%lld %d is sleeping\n", (timestamp() - philo->main->start), philo->num);
	if (philo->main->end == 1)
		philo->status = death;
	pthread_mutex_unlock(&philo->main->endmutex);
}

int routine_lock(t_philo *philo)
{
	if (philo->main->nphilos % 2)
	{
		if (pthread_mutex_lock(philo->left)&& !philo->main->end)
			return (1);
		routine_print(philo, idle);
		if (philo->main->nphilos == 1)
		{
			pthread_mutex_unlock(philo->left);
			return (1);
		}
		if(pthread_mutex_lock(philo->right) && !philo->main->end)
			return (1);
		routine_print(philo, idle);
	}
	else 
	{
		if (pthread_mutex_lock(philo->right)&& !philo->main->end)
			return (1);
		routine_print(philo, idle);
		if(pthread_mutex_lock(philo->left) && !philo->main->end)
			return (1);
		routine_print(philo, idle);
	}
	return (0);
}

int routine_unlock(t_philo *philo)
{
	if (!philo->main->nphilos % 2)
	{
		if (pthread_mutex_unlock(philo->left))
			return (1);
		if (pthread_mutex_unlock(philo->right))
			return (1);
	}
	else 
	{
		if (pthread_mutex_unlock(philo->right))
			return (1);
		if (pthread_mutex_unlock(philo->left))
			return (1);
	}
	return (0);
}

void *routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	philo->tv = timestamp();
	pthread_mutex_lock(&philo->main->endmutex);
	pthread_mutex_unlock(&philo->main->endmutex);
	while (1)
	{
		if (philo->status == death)
			return (NULL);
		if (routine_lock(philo))
			return (NULL);
		routine_print(philo, eating);
		philo_usleep(philo->main->eat);
		if (routine_unlock(philo))
			return (NULL);
		if (philo->lunches == philo->max_lunch)
			return (NULL);
		routine_print(philo, sleeping);
		if (philo->status == death)
			return (NULL);
		philo_usleep(philo->main->sleep);
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


void thread_monitor_terminate_threads(t_data *data, int num, long long stamp)
{
	int index;

	index = 0;
	data->end = 1;
	printf("%lld %d died\n", (stamp - data->start), data->philos[num]->num);
}

void thread_monitor(t_data *data)
{
	int	index;
	long long stamp;

	index = 0;
	while (1)
	{
		stamp = timestamp();
		if ((stamp - data->philos[index]->tv) > data->die && data->philos[index]->tv)
			return (thread_monitor_terminate_threads(data, index, stamp));
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
	pthread_mutex_lock(&data->endmutex);
	while (index < data->nphilos)
	{
		if (thread_init_philo(data, index))
			return (1);
		index++;
	}
	data->start = timestamp();
	pthread_mutex_unlock(&data->endmutex);
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
	if (pthread_mutex_destroy(&data->endmutex) != 0)
		return (error(data, "mutex destroy end error\n", 1));
	return (0);
}


int main(int argc, char **argv)
{
	t_data data;

	if (argc < 5 || argc > 6)
		return (error(&data, "wrong amount of arguments\n", 1));
	if (philos_init(&data, argv))
		return (error(&data, "", 1));
	if (data.nphilos == 0)
		return (error(&data, "there cannot be 0 philos\n", 1));
	if (mutex_init(&data))
		return (error(&data, "", 1));
	if (thread_init(&data))
		return (error(&data, "", 1));
	if (wait_threads(&data))
		return (error(&data, "", 1));
	free_all(&data);
	return (0);
}
