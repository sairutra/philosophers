/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:04:28 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/20 18:38:16 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		print_err("wrong amount of arguments\n");
		return (1);
	}
	if (argv[5])
	{
		if (philo_atoi(argv[5]) == 0)
			return (error(NULL, "there cannot be 0 meals\n", 1));
	}
	if (data_init(&data, argv))
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
