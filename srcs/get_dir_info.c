/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 18:03:36 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 20:30:38 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	malloc_content(char *path, char ***content)
{
	DIR				*dir = opendir(path);
	if (!dir) {
		ft_putstr_fd(RED "ft_ls: cannot access '", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd("': No such file or directory\n" RESET, 2);
		exit(2);
	}

	struct dirent	*entry;
	size_t			count = 0;

	while (1) {
		entry = readdir(dir);
		if (!entry)
			break ;
		count++;
	}
	(*content) = malloc(sizeof(char *) * (count + 1));
	closedir(dir);
	return count;
}

static void	get_content(char *path, char ***content)
{
	DIR				*dir = opendir(path);
	if (!dir) {
		ft_putstr_fd(RED "ft_ls: cannot access '", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd("': No such file or directory\n" RESET, 2);
		exit(2);
	}

	struct dirent	*entry;
	size_t			count = 0;

	while (1) {
		entry = readdir(dir);
		if (!entry)
			break ;
		(*content)[count] = ft_strdup(entry->d_name);
		count++;
	}
	(*content)[count] = NULL;
	closedir(dir);
}

static void	sort_content(int size, char ***content)
{
	char	*tmp;

	for (int i = 0; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			if (ft_strcasecmp((*content)[i], (*content)[j]) > 0) {
				tmp = (*content)[i];
				(*content)[i] = (*content)[j];
				(*content)[j] = tmp;
			}
		}
	}
}

t_dir_info	*get_dir_info(char *path)
{
	char	**content;

	t_dir_info		*dir_info = malloc(sizeof(t_dir_info));
	dir_info->size = malloc_content(path, &content);
	get_content(path, &content);
	sort_content(dir_info->size, &content);
	dir_info->content = content;
	return dir_info;
}
