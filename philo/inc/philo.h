/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:15:13 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/13 18:41:16 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t thread;
	pthread_mutex_t *left;
	pthread_mutex_t *right;
	int	die;
	int	eat;
	int	sleep;
	int	lunches;
	int	max_lunch;
	struct timeval tv;
}	t_philo;

typedef struct s_data
{
	t_philo **philos;
	pthread_mutex_t *forks;
	int	nphilos;
	int	nforks;
	int	die;
	int	eat;
	int	sleep;
	int	lunches;
}	t_data;

#endif	// BUILTINS_H
