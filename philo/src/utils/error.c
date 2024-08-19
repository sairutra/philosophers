/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 16:58:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 18:13:37 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	free_all(t_data *data)
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

int	error(t_data *data, char *msg, int exitcode)
{
	print_err(msg);
	free_all(data);
	return (exitcode);
}

void	print_err(char *msg)
{
	int	index;

	index = 0;
	while (msg[index] != '\0')
		index++;
	write(2, msg, index);
}
