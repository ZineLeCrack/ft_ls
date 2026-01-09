/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 10:18:09 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 15:22:32 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int	ft_strcasecmp(const char *s1, const char *s2)
{
	unsigned char	c1;
	unsigned char	c2;

	while (*s1 == '.')
		s1++;
	while (*s2 == '.')
		s2++;
	while (*s1 || *s2) {
		c1 = (unsigned char)*s1;
		c2 = (unsigned char)*s2;
		if (c1 >= 'A' && c1 <= 'Z')
			c1 += 32;
		if (c2 >= 'A' && c2 <= 'Z')
			c2 += 32;
		if (c1 != c2)
			return (c1 - c2);
		if (*s1)
			s1++;
		if (*s2)
			s2++;
	}
	return 0;
}
