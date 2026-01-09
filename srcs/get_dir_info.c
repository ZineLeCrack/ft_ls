/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 18:03:36 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 11:22:19 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	malloc_content(char *path, struct dirent ***content)
{
	DIR				*dir = opendir(path);
	if (!dir) {
		ft_putstr_fd(RED "Error: can't open pwd\n" RESET, 2); exit(1);
	}

	struct dirent	*entry;
	size_t			count = 0;

	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		count++;
	}
	(*content) = malloc(sizeof(struct dirent *) * (count + 1));
	closedir(dir);
	return count;
}

static DIR	*get_content(char *path, struct dirent ***content)
{
	DIR				*dir = opendir(path);
	if (!dir) {
		ft_putstr_fd(RED "Error: can't open pwd\n" RESET, 2); exit(1);
	}

	struct dirent	*entry;
	size_t			count = 0;

	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		(*content)[count] = entry;
		count++;
	}
	(*content)[count] = NULL;
	return dir;
}

static void	sort_content(int size, struct dirent ***content)
{
	struct dirent	*tmp;

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (ft_strcasecmp((*content)[i]->d_name, (*content)[j]->d_name) > 0)
			{
				tmp = (*content)[i];
				(*content)[i] = (*content)[j];
				(*content)[j] = tmp;
			}
		}
	}
}

t_dir_info	*get_dir_info(char *path)
{
	struct dirent	**content;

	t_dir_info		*dir_info = malloc(sizeof(t_dir_info));
	dir_info->size = malloc_content(path, &content);
	dir_info->dir = get_content(path, &content);
	sort_content(dir_info->size, &content);
	dir_info->content = content;
	return dir_info;
}
