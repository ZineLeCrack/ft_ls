/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 10:18:09 by romain            #+#    #+#             */
/*   Updated: 2026/01/11 12:25:33 by romain           ###   ########.fr       */
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

int	is_dir(char *path)
{
	struct stat	*st = malloc(sizeof(struct stat));
	if (!st) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return ERROR;
	}
	if (stat(path, st) == -1) {
		free(st);
		ft_putstr_fd(RED "ft_ls: cannot access '", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd("': No such file or directory\n" RESET, 2);
		return ERROR;
	}
	int	is_dir = S_ISDIR(st->st_mode);
	free(st);
	return is_dir ? TRUE : FALSE;
}

void	reverse_content(int size, char ***content)
{
	char	*tmp;

	for (int i = 0; i < size / 2; i++) {
		tmp = (*content)[i];
		(*content)[i] = (*content)[size - i - 1];
		(*content)[size - i - 1] = tmp;
	}
}
