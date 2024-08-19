/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 17:11:36 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 18:05:44 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	routine_sync(t_philo *philo)
{
	pthread_mutex_lock(&philo->main->endmutex);
	pthread_mutex_unlock(&philo->main->endmutex);
	pthread_mutex_lock(&philo->own);
	philo->tv = timestamp();
	pthread_mutex_unlock(&philo->own);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	routine_sync(philo);
	philo->tv = timestamp();
	if (philo->num % 2 && philo->main->nphilos != 1)
		philo_usleep(philo, philo->main->eat / 2);
	while (1)
	{
		if (philo->status == death)
			return (NULL);
		if (routine_lock(philo))
			return (NULL);
		routine_print(philo, eating);
		philo_usleep(philo, philo->main->eat);
		if (routine_unlock(philo))
			return (NULL);
		if (philo->status == finished)
			return (NULL);
		routine_print(philo, sleeping);
		if (philo_usleep(philo, philo->main->sleep))
			return (NULL);
		routine_print(philo, thinking);
	}
	return (NULL);
}
