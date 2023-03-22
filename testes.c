/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joao-per <joao-per@student.42lisboa.com>   +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/03/18 15:07:13 by joao-per          #+#    #+#             */
/*   Updated: 2023/03/18 15:07:13 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


unsigned long	get_timestamp(unsigned long t_start)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - t_start);
}

void	print_state(t_philo *philo, const char *state)
{
	pthread_mutex_lock(&philo->info->print);
	if (!philo->info->death_occurred)
	{
		printf("%lu %d %s\n", get_timestamp(philo->info->t_start), philo->id,
			state);
	}
	pthread_mutex_unlock(&philo->info->print);
}

int	should_philosopher_die(t_philo *philo)
{
	int	last_eated;

	pthread_mutex_lock(&philo->info->print);
	last_eated = get_timestamp(philo->t_start) - philo->last_meal;
	pthread_mutex_unlock(&philo->info->print);
	if (last_eated >= philo->time_to_die)
	{
		pthread_mutex_lock(&philo->info->print);
		if (!philo->info->death_occurred)
		{
			philo->info->death_occurred = 1;
			printf("%lu %d died\n", get_timestamp(philo->info->t_start),
				philo->id);
		}
		pthread_mutex_unlock(&philo->info->print);
		return (1);
	}
	return (0);
}

int	check_nbr_eats(t_philo *philo)
{
	int	nbr_eats;

	pthread_mutex_lock(&philo->info->print);
	nbr_eats = philo->eat_count;
	pthread_mutex_unlock(&philo->info->print);
	if (nbr_eats < philo->max_eat || philo->max_eat == -1)
		return (0);
	philo->info->max_eats = 1;
	return (1);
}

void	*philo_thread(void *arg)
{
	t_philo *philo;
	int death_occurred;
	int max_eats;
	int num_philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->info->print);
	philo->last_meal = get_timestamp(philo->info->t_start);
	pthread_mutex_unlock(&philo->info->print);
	num_philo = philo->info->num_philo;

	while (1)
	{
		pthread_mutex_lock(&philo->info->print);
		death_occurred = philo->info->death_occurred;
		max_eats = philo->info->max_eats;
		pthread_mutex_unlock(&philo->info->print);
		if (death_occurred || max_eats)
			break ;
		if (num_philo == 1)
		{
			printf("%lu %d has taken a fork\n",
			get_timestamp(philo->info->t_start), philo->id);
			usleep(philo->time_to_die * 1000);
			break ;

		}
		if (philo->id % 2 == 0)
			usleep(1000);
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&philo->info->print);
		philo->last_meal = get_timestamp(philo->t_start);
		philo->eat_count++;
		pthread_mutex_unlock(&philo->info->print);
		print_state(philo, "is eating");
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		print_state(philo, "is sleeping");
		usleep(philo->time_to_sleep * 1000);
		print_state(philo, "is thinking");
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	pthread_mutex_t *forks;
	pthread_t *threads;
	t_philo *philo;
	t_info info;
	int i;
	int death;
	int max_eats;
	int num_philo;
	unsigned long t_start;
	pthread_mutex_t print_mutex;

	pthread_mutex_init(&info.print, NULL);
	info.t_start = get_timestamp(0);
	info.death_occurred = 0;
	info.max_eats = 0;
	death = 0;
	max_eats = 0;
	num_philo = ft_atol(av[1]);
	info.num_philo = num_philo;
	threads = (pthread_t *)malloc(num_philo * sizeof(pthread_t));
	philo = (t_philo *)malloc(num_philo * sizeof(t_philo));
	forks = (pthread_mutex_t *)malloc(num_philo * sizeof(pthread_mutex_t));
	philo->is_dead = 0;
	if (ac < 5 || ac > 6)
	{
		printf("Incorrect number of arguments\n");
		return (1);
	}
	i = 0;
	while (i < num_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	t_start = get_timestamp(0);
	pthread_mutex_init(&print_mutex, NULL);
	i = 0;
	while (i < num_philo)
	{
		philo[i].id = i + 1;
		philo[i].time_to_die = ft_atol(av[2]);
		philo[i].time_to_eat = ft_atol(av[3]);
		philo[i].time_to_sleep = ft_atol(av[4]);
		philo[i].eat_count = 0;
		if (ac == 6)
			philo[i].max_eat = ft_atol(av[5]);
		else
			philo[i].max_eat = -1;
		if (i + 1 != num_philo)
		{
			philo[i].left_fork = &forks[i];
			philo[i].right_fork = &forks[(i + 1) % num_philo];
		}
		else
		{
			philo[i].left_fork = &forks[(i + 1) % num_philo];
			philo[i].right_fork = &forks[i];
		}
		philo[i].t_start = t_start;
		philo[i].print = &print_mutex;
		philo[i].info = &info;
		if (pthread_create(&threads[i], NULL, philo_thread, &philo[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (1 && !max_eats && !death)
	{
		i = 0;
		while (i < num_philo)
		{
			if (should_philosopher_die(&philo[i]))
				death = 1;
			if (check_nbr_eats(&philo[i]))
				max_eats = 1;
			i++;
		}
	}
	i = 0;
	while (i < num_philo)
	{
		if (pthread_join(threads[i], NULL) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < num_philo)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(threads);
	free(forks);
	free(philo);
}
