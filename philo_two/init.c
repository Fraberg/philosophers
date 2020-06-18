/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:39:14 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/18 17:01:39 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Retourne un string équivalent à name+id pour différencier
** les N sémaphores
** L'int eat vaut 0 si on doit retourner un name philo-id
** ou alors 1 si philo-eat-id
*/

void		semanames(char *name, int id, int eat)
{
	int			i;
	const char	basename1[6] = "philo-";
	const char	basename2[10] = "philo-eat-";

	i = -1;
	if (eat)
		while (++i < 10)
			name[i] = basename2[i];
	else
		while (++i < 6)
			name[i] = basename1[i];
	while (id)
	{
		name[i++] = (id % 10) + '0';
		id /= 10;
	}
	name[i] = '\0';
}

int				initphilos(void)
{
	int			i;
	char		name[50];

	i = 0;
	while (i < g_banquet.nb_philos)
	{
		g_banquet.philos[i].pos = i;
		g_banquet.philos[i].last_meal = 0;
		g_banquet.philos[i].meal_count = 0;
		semanames(name, i + 1, PHI_INIT);
		sem_unlink(name);
		if ((g_banquet.philos[i].eating =
			sem_open(name, O_CREAT, 0666, 1)) == SEM_FAILED)
			return (RET_ERROR);
		semanames(name, i + 1, EAT_INIT);
		sem_unlink(name);
		if ((g_banquet.philos[i].meat_count =
			sem_open(name, O_CREAT, 0666, 0)) == SEM_FAILED)
			return (RET_ERROR);
		i++;
	}
	return (RET_SUCCESS);
}

int				initsemas(int philonum)
{
	if ((g_banquet.forks =
		sem_open(FORKS, O_CREAT, 0666, philonum)) == SEM_FAILED)
		return (RET_ERROR);
	if ((g_banquet.ask_forks =
		sem_open(ASKTAKEFORKS, O_CREAT, 0666, 1)) == SEM_FAILED)
		return (RET_ERROR);
	if ((g_banquet.write =
		sem_open(WRITE, O_CREAT, 0666, 1)) == SEM_FAILED)
		return (RET_ERROR);
	if ((g_banquet.death =
		sem_open(DEATH, O_CREAT, 0666, 0)) == SEM_FAILED)
		return (RET_ERROR);
	return (RET_SUCCESS);
}

/*
** Named semaphores content cannot be printed !
**
** To show context
** printf("NAME PHILO %d : %s\n", i, name);
** printf("NAME PHILO %d : %s\n", i, name);
** int i = 0;
** while (i < context.philosophers)
** {
** printf("philo %d pos : %d\n", i, context.philos[i].pos);
** printf("philo %d meal_count : %d\n", i, context.philos[i].meal_count);
** i++;
** }
*/

int				parse_banquet_config(int ac, char **av)
{
	ft_clean();
	memset(&g_banquet, 0, sizeof(g_banquet));
	g_banquet.nb_philos = ft_atoi(av[1]);
	g_banquet.time_to_die = ft_atoi(av[2]);
	g_banquet.time_to_eat = ft_atoi(av[3]);
	g_banquet.time_to_sleep = ft_atoi(av[4]);
	g_banquet.max_eat = (ac == 6) ? ft_atoi(av[5]) : 0;
	if (g_banquet.nb_philos < 2 || g_banquet.nb_philos > 200 ||
		g_banquet.time_to_die < 60 || g_banquet.time_to_eat < 60 ||
		g_banquet.time_to_sleep < 60 || g_banquet.max_eat < 0)
		return (RET_ERROR);
	g_banquet.philos = NULL;
	g_banquet.globaleatcoutner = 0;
	if (!(g_banquet.philos = malloc(sizeof(t_philo) * g_banquet.nb_philos)))
		return (RET_ERROR);
	if (initphilos())
		return (RET_ERROR);
	if (initsemas(g_banquet.nb_philos))
		return (RET_ERROR);
	return (RET_SUCCESS);
}
