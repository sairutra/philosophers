/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:04:28 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 18:13:48 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (error(&data, "wrong amount of arguments\n", 1));
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
