/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 10:18:09 by romain            #+#    #+#             */
/*   Updated: 2026/01/28 17:30:53 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char	*get_next_path(char *path, char *dir)
{
	char	*next_path;

	next_path = malloc(ft_strlen(path) + ft_strlen(dir) + 2);
	if (!next_path) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return NULL;
	}
	ft_strlcpy(next_path, path, ft_strlen(path) + 1);
	next_path[ft_strlen(path)] = '/';
	ft_strlcpy(next_path + ft_strlen(path) + 1, dir, ft_strlen(dir) + 1);
	return next_path;
}

int	ft_strcasecmp(const char *s1, const char *s2)
{
	unsigned char	c1;
	unsigned char	c2;

	while (*s1 || *s2) {
		while (*s1 && !ft_isalnum(*s1))
			s1++;
		while (*s2 && !ft_isalnum(*s2))
			s2++;
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
	if (lstat(path, st) == -1) {
		free(st);
		if (errno == ENOENT)
			return NSFOD;
		if (errno == EACCES)
			return NO_PERM;
	}
	int	is_dir = S_ISDIR(st->st_mode);
	free(st);
	return is_dir ? TRUE : FALSE;
}

void	sort_content(int size, char ***content, char *path, unsigned char options)
{
	if (TIME_OPT(options)) {
		struct stat	*st = malloc(sizeof(struct stat));
		if (!st) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			return ;
		}
		unsigned long	*times = malloc(sizeof(time_t) * size);
		if (!times) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			free(st);
			return ;
		}
		for (int i = 0; i < size; i++) {
			char *next_path = get_next_path(path, (*content)[i]);
			if (!next_path) {
				ft_putstr_fd(RED "Fatal error\n" RESET, 2);
				free(times);
				free(st);
				return ;
			}
			lstat(next_path, st);
			times[i] = (unsigned long)st->st_mtim.tv_sec * 1000000000UL + (unsigned long)st->st_mtim.tv_nsec;
			free(next_path);
		}
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				if (times[i] < times[j]) {
					time_t	time_tmp = times[i];
					char	*content_tmp = (*content)[i];
					times[i] = times[j];
					(*content)[i] = (*content)[j];
					times[j] = time_tmp;
					(*content)[j] = content_tmp;
				}
			}
		}
		free(st);
		free(times);
	} else {
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				int	val = ft_strcasecmp((*content)[i], (*content)[j]);
				if (val > 0) {
					char	*tmp = (*content)[i];
					(*content)[i] = (*content)[j];
					(*content)[j] = tmp;
				} else if (val == 0 && ft_strcmp((*content)[i], (*content)[j]) > 0) {
					char	*tmp = (*content)[i];
					(*content)[i] = (*content)[j];
					(*content)[j] = tmp;
				}
			}
		}
	}
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
