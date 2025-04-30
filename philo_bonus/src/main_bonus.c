/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:42:35 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/30 19:10:28 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philo;

	philo = NULL;
	if (parse_arguments(argc, argv, &data))
		return (1);
	if (init_semaphore(&data))
	{
		cleanup_semaphore(&data);
		unlink_semaphore();
		error("Error creating semaphores");
	}
	init_philo(&data, &philo);
	start_simulation(&data, philo);
	wait_philo(&data, philo);
	return (0);
}
