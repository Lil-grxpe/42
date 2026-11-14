/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_int_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lil_grxpe <lil_grxpe@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 15:59:28 by lil_grxpe         #+#    #+#             */
/*   Updated: 2026/11/14 16:23:59 by lil_grxpe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_sort_int_tab(int *tab, int size)
{
	int	i;
	int	temp;

	i = 0;
	while (i < size - 1)
	{
		if (*(tab + i) > *(tab + i + 1))
		{
			temp = *(tab + i);
			*(tab + i) = *(tab + i + 1);
			*(tab + i + 1) = temp;
			i = 0;
		}
		else
		{
			i++;
		}
	}
}
