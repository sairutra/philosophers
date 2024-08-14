/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:15:13 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/14 16:37:19 by spenning      ########   odam.nl         */
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

enum e_status
{
	idle,
	eating,
	thinking,
	sleeping,
	finished,
	death
}	;

typedef struct s_data t_data;

typedef struct s_philo
{
	pthread_t thread;
	pthread_mutex_t *left;
	pthread_mutex_t *right;
	t_data * main;
	int	lunches;
	int	max_lunch;
	int	num;
	int status;
	long long tv;
}	t_philo;

typedef struct s_data
{
	t_philo **philos;
	pthread_mutex_t *forks;
	pthread_mutex_t endmutex;
	int	nphilos;
	int	nforks;
	int	end;
	int	die;
	int	eat;
	int	sleep;
	int	lunches;
	long long start;
}	t_data;

#endif