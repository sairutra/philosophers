/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lib.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/19 16:58:11 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/19 17:00:10 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	philo_len(const char *arg)
{
	int	index;

	index = 0;
	while (arg[index] != '\0')
		index++;
	return (index);
}

int	philo_strncmp(const char *s1, const char *s2, size_t n)
{
	int		retvalue;
	size_t	index;
	size_t	s1_len;
	size_t	s2_len;

	retvalue = 0;
	index = 0;
	s1_len = philo_len(s1);
	s2_len = philo_len(s2);
	while (index < n && (index < s1_len || index < s2_len))
	{
		retvalue += ((unsigned char)s1[index] - (unsigned char)s2[index]);
		index++;
	}
	return (retvalue);
}

int	philo_atoi(char *arg)
{
	int	result;
	int	index;

	index = 0;
	result = 0;
	if (philo_len(arg) > 10)
	{
		print_err("integer overflow: ");
		return (-1);
	}
	if (philo_strncmp(arg, "2147483647", 10) > 0 && philo_len(arg) == 10)
	{
		print_err("integer overflow: ");
		return (-1);
	}
	while (arg[index] != 0)
	{
		if (!((arg[index] > 47) && (arg[index] < 58)))
			return (-1);
		result = result * 10 + (arg[index] - 48);
		index++;
	}
	return (result);
}
