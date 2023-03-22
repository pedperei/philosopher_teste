/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 23:36:52 by joao-per          #+#    #+#             */
/*   Updated: 2023/03/19 23:36:52 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

int	is_digit(int character)
{
	if (character >= '0' && character <= '9')
		return (1);
	return (0);
}

long	ft_atol(const char *str)
{
	long int	result;
	int			i;
	int			negative;

	i = 0;
	negative = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			negative = -1;
		i++;
	}
	while (is_digit(str[i]))
	{
		result = (str[i] - 48) + (result * 10);
		i++;
	}
	if (negative == -1)
		result = -result;
	if (result > INT_MAX || result < INT_MIN)
		return (6969696969);
	return (result);
}