/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 18:03:36 by romain            #+#    #+#             */
/*   Updated: 2026/01/10 14:41:12 by romain           ###   ########.fr       */
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

static int	is_dir(char *path)
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

static void	reverse_content(int size, char ***content)
{
	char	**new_content = malloc(sizeof(char *) * (size + 1));
	if (!new_content) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return ;
	}

	for (int i = 0; i < size; i++) {
		new_content[i] = (*content)[size - i - 1];
	}
	new_content[size] = NULL;

	free(*content);
	(*content) = new_content;
}

t_dir_info	*get_dir_info(char *path, char *abs_path, unsigned char options)
{
	char	**content;

	int	status = is_dir(abs_path);
	if (status == TRUE) {
		t_dir_info		*dir_info = malloc(sizeof(t_dir_info));
		if (!dir_info) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			return NULL;
		}
		dir_info->size = malloc_content(abs_path, &content);
		if (dir_info->size == -1) {
			free(dir_info);
			return NULL;
		}
		if  (get_content(abs_path, &content) == -1) {
			free(content);
			free(dir_info);
			return NULL;
		}
		sort_content(dir_info->size, &content);
		if (REVERSE_OPT(options)) {
			reverse_content(dir_info->size, &content);
			if (!content) {
				for (int i = 0; content[i]; i++)
					free(content[i]);
				free(content);
				free(dir_info);
				return NULL;
			}
		}
		dir_info->content = content;
		dir_info->is_dir = 1;
		return dir_info;
	} else if (status == FALSE) {
		t_dir_info		*dir_info = malloc(sizeof(t_dir_info));
		if (!dir_info) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			return NULL;
		}
		dir_info->size = 1;
		content = malloc(sizeof(char *) * 2);
		if (!content) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			free(dir_info);
			return NULL;
		}
		content[0] = ft_strdup(path);
		if (!content[0]) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			free(content);
			free(dir_info);
			return NULL;
		}
		content[1] = NULL;
		dir_info->content = content;
		dir_info->is_dir = 0;
		return dir_info;
	} else {
		return NULL;
	}
}
