/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 14:15:13 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 17:41:30 by spenning      ########   odam.nl         */
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
	pickfork,
	eating,
	thinking,
	sleeping,
	finished,
	alive,
	death
};

typedef struct s_data t_data;

typedef struct s_philo
{
	pthread_t thread;
	pthread_mutex_t *left;
	pthread_mutex_t *right;
	pthread_mutex_t own;
	t_data * main;
	long long start;
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


//UTILS
/**
 * @brief This function frees the data structure if called
 * @param data 
 */
void		free_all(t_data *data);
/**
 * @brief This function frees data, prints msg, and returns exit code
 * @param data 
 * @param msg 
 * @param exitcode 
 * @return int 
 */
int			error(t_data *data, char *msg, int exitcode);
/**
 * @brief this the equivalent of ft_strlen, strlen for this project.
 * @param arg 
 * @return int 
 */
int			philo_len(const char *arg);
/**
 * @brief this is the equivalent of ft_strncmp, strncmp for this project
 * @param s1 
 * @param s2 
 * @param n 
 * @return int 
 */
int			philo_strncmp(const char *s1, const char *s2, size_t n);
/**
 * @brief this is the equivalent of ft_atoi for this project
 * @param s1 
 * @param s2 
 * @param n 
 * @return int 
 */
int			philo_atoi(char *arg);
/**
 * @brief this function writes msg to strerr
 * @param msg 
 */
void		print_err(char * msg);
/**
 * @brief this function returns a timestamp in milliseconds in the form 
 * a long long
 * @return long long 
 */
long long	timestamp(void);
/**
 * @brief this function is the usleep equivalent for this project,
 * which is more accurate plus function exists when a philo has died, 
 * so there is no long wait when a long param has been passed.
 * if a philo has died in the meanwhile then function returns 1, otherwise 0
 * @param t_philo* philo 
 * @param long long mili 
 */
int			philo_usleep(t_philo* philo, long long mili);

//INIT
/**
 * @brief this function initialize the main data structure. returns 1 on error, 
 * and 0 on successful operation 
 * @param data 
 * @param argv 
 * @return int 
 */
int			data_init(t_data *data, char **argv);
/**
 * @brief this function initialize the main data structure. returns 1 on error, 
 * and 0 on successful operation 
 * @param data 
 * @param argv 
 * @return int 
 */
int			mutex_init(t_data *data);

//MONITOR
/**
 * @brief this function monitors the philos, to know if they are finished, 
 * or if a philo has died
 * @param data 
 */
void		thread_monitor(t_data *data);

//ROUTINE
/**
 * @brief this function will unlock the mutexex/forks of the philos,
 * return 1 on error
 * @param philo 
 * @return int 
 */
int			routine_unlock(t_philo *philo);
/**
 * @brief this function will lock the mutexex/forks of the philos,
 * returns 1 on error
 * @param philo 
 * @return int 
 */
int			routine_lock(t_philo *philo);
/**
 * @brief this function prints the messages of the philos
 * @param philo 
 * @return int 
 */
void		routine_print(t_philo *philo, int status);
/**
 * @brief this function checks if main thread has initialized end of simulation
 * value, returns 1 if end of simulation is called, 0 if not.
 * @param philo 
 * @return int 
 */
int			routine_death(t_philo *philo);
/**
 * @brief this function is the routine loop of the philos
 * @param arg 
 * @return void* 
 */
void		*routine(void *arg);
/**
 * @brief this function initializes all the threads/philos
 * @param arg 
 * @return void* 
 */
int			thread_init(t_data *data);
/**
 * @brief this function waits for all the threads to finish, 
 * and clears all mutexes.
 * @param arg 
 * @return void* 
 */
int			wait_threads(t_data *data);


#endif