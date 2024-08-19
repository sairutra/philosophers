/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 17:12:36 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 17:35:02 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int routine_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->main->endmutex);
	pthread_mutex_lock(&philo->own);
	if (philo->main->end == 1)
		philo->status = death;
	pthread_mutex_unlock(&philo->own);
	pthread_mutex_unlock(&philo->main->endmutex);
	if (philo->status == death)
		return (1);
	return (0);
}

void routine_print(t_philo *philo, int status)
{
	pthread_mutex_lock(&philo->main->endmutex);
	pthread_mutex_lock(&philo->own);
	philo->status = status;
	if (status == eating && philo->main->end == 0)
	{
		philo->tv = timestamp();
		printf("%lld %d is eating\n", (philo->tv - philo->main->start), philo->num);
		philo->lunches++;
		if (philo->lunches == philo->max_lunch)
			philo->status = finished;
	}
	else if (status == pickfork && philo->main->end == 0)
		printf("%lld %d has taken a fork\n", (timestamp() - philo->main->start), philo->num);
	else if (status == thinking && philo->main->end == 0)
		printf("%lld %d is thinking\n", (timestamp() - philo->main->start), philo->num);
	else if (status == sleeping && philo->main->end == 0)
		printf("%lld %d is sleeping\n", (timestamp() - philo->main->start), philo->num);
	if (philo->main->end == 1)
		philo->status = death;
	pthread_mutex_unlock(&philo->own);
	pthread_mutex_unlock(&philo->main->endmutex);
}

int routine_lock(t_philo *philo)
{
	if (pthread_mutex_lock(philo->right))
		return (1);
	routine_print(philo, pickfork);
	if (philo->main->nphilos == 1)
	{
		pthread_mutex_unlock(philo->right);
		return (1);
	}
	if (pthread_mutex_lock(philo->left))
	{
		pthread_mutex_unlock(philo->right);
		return (1);
	}
	routine_print(philo, pickfork);
	return (0);
}

int routine_unlock(t_philo *philo)
{
	if (pthread_mutex_unlock(philo->right))
	{
		pthread_mutex_unlock(philo->left);
		return (1);
	}
	if (pthread_mutex_unlock(philo->left))
		return (1);
	return (0);
}
