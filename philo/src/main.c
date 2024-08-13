/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:04:28 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/13 15:28:37 by spenning      ########   odam.nl         */
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


int error(char *msg, int exitcode)
{
	print_err(msg);
	return (exitcode);
}


int	philos_init_var(int* data_var, char *var, char *err_msg)
{
	*data_var = philo_atoi(var);
	if (*data_var == -1)
		return (error(err_msg, 1));
	return (0);
}

int	philos_init(t_philos *data, char **argv)
{
	memset(data, 0, sizeof(t_philos));
	if (philos_init_var(&data->nphilos, argv[1], "number of philos wrong\n"))
		return (1);
	data->forks = data->nphilos;
	if (philos_init_var(&data->die, argv[2], "time to die wrong\n"))
			return (1);
	if (philos_init_var(&data->eat, argv[3], "time to eat wrong\n"))
			return (1);
	if (philos_init_var(&data->sleep, argv[4], "time to sleep wrong\n"))
			return (1);
	if (argv[5])
	{
		if (philos_init_var(&data->lunches, argv[5], "amount of lunches wrong\n"))
			return (1);
	}
	if (data->nphilos >= 200)
		return (error("too many philos, not more than 200 allowed\n", 1));
	if (data->die <= 60)
		return (error("time to die too low, not lower than 60\n", 1));
	if (data->eat <= 60)
		return (error("time to eat too low, not lower than 60\n", 1));
	if (data->sleep <= 60)
		return (error("time to sleep too low, not lower than 60\n", 1));
	return (0);
}



int main(int argc, char **argv)
{
	t_philos data;

	if (argc < 5 || argc > 6)
		return (error("wrong amount of arguments\n", 1));
	if (philos_init(&data, argv))
		return (1);
	
}
