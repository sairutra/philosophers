/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 17:05:20 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 18:12:58 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

long long	timestamp(void)
{
	long long		ret;
	struct timeval	stamp;

	gettimeofday(&stamp, NULL);
	ret = (stamp.tv_sec * 1000) + (stamp.tv_usec / 1000);
	return (ret);
}

int	philo_usleep(t_philo *philo, long long mili)
{
	long long	wakeup;

	wakeup = mili + timestamp();
	while (wakeup > timestamp())
	{
		usleep(200);
		if (routine_death(philo))
			return (1);
	}
	return (0);
}
