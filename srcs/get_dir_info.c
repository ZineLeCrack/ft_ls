/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 18:03:36 by romain            #+#    #+#             */
/*   Updated: 2026/01/11 12:39:10 by romain           ###   ########.fr       */
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
		return -1;
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
	if (!(*content)) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		closedir(dir);
		return -1;
	}
	closedir(dir);
	return count;
}

static int	get_content(char *path, char ***content)
{
	DIR				*dir = opendir(path);
	if (!dir) {
		ft_putstr_fd(RED "ft_ls: cannot access '", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd("': No such file or directory\n" RESET, 2);
		return -1;
	}

	struct dirent	*entry;
	size_t			count = 0;

	while (1) {
		entry = readdir(dir);
		if (!entry)
			break ;
		(*content)[count] = ft_strdup(entry->d_name);
		if (!(*content)[count]) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			closedir(dir);
			return -1;
		}
		count++;
	}
	(*content)[count] = NULL;
	closedir(dir);
	return 0;
}

t_dir_info	*get_dir_info(char *path, unsigned char options)
{
	char	**content;

	t_dir_info		*dir_info = malloc(sizeof(t_dir_info));
	if (!dir_info) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return NULL;
	}
	dir_info->size = malloc_content(path, &content);
	if (dir_info->size == -1) {
		free(dir_info);
		return NULL;
	}
	if  (get_content(path, &content) == -1) {
		free(content);
		free(dir_info);
		return NULL;
	}
	sort_content(dir_info->size, &content);
	if (REVERSE_OPT(options)) {
		reverse_content(dir_info->size, &content);
	}
	dir_info->content = content;
	dir_info->is_dir = 1;
	return dir_info;
}
