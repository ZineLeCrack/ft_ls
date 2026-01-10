/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directories.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 09:40:02 by romain            #+#    #+#             */
/*   Updated: 2026/01/10 14:35:05 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static char	*get_abs_path(char *pwd, char *path, unsigned char options, int put_dir_name)
{
	static int	first = 1;
	char		*abs_path;

	if (!first)
		ft_printf("\n");
	else
		first = 0;
	abs_path = malloc(ft_strlen(pwd) + ft_strlen(path) + 2);
	if (!abs_path) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return NULL;
	}
	ft_strlcpy(abs_path, pwd, ft_strlen(pwd) + 1);
	abs_path[ft_strlen(pwd)] = '/';
	ft_strlcpy(abs_path + ft_strlen(pwd) + 1, path, ft_strlen(path) + 1);
	if (RECURSIVE_OPT(options) || put_dir_name)
		ft_printf("%s:\n", path);
	return abs_path;
}

static char	*get_next_abs_path(char *abs_path, char *dir)
{
	char	*next_abs_path;

	next_abs_path = malloc(ft_strlen(abs_path) + ft_strlen(dir) + 2);
	if (!next_abs_path) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return NULL;
	}
	ft_strlcpy(next_abs_path, abs_path, ft_strlen(abs_path) + 1);
	next_abs_path[ft_strlen(abs_path)] = '/';
	ft_strlcpy(next_abs_path + ft_strlen(abs_path) + 1, dir,
		ft_strlen(dir) + 1);
	return next_abs_path;
}

static char	*get_next_path(char *path, char *dir)
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

static int	launch_recursion(t_dir_info *dir_info, char *pwd,
	char *abs_path, char *path, unsigned char options, int put_dir_name)
{
	struct stat	*st;
	char		*next_abs_path;
	char		*next_path;

	st = malloc(sizeof(struct stat));
	if (!st) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return ERROR;
	}
	for (int i = 0; i < dir_info->size; i++) {
		if (dir_info->content[i][0] != '.' || ALL_OPT(options)) {
			next_abs_path = get_next_abs_path(abs_path, dir_info->content[i]);
			if (!next_abs_path) {
				free(st);
				return ERROR;
			}
			stat(next_abs_path, st);
			if (S_ISDIR(st->st_mode)
			&& ft_strcmp(dir_info->content[i], ".")
			&& ft_strcmp(dir_info->content[i], ".."))
			{
				next_path = get_next_path(path, dir_info->content[i]);
				if (!next_path) {
					free(st);
					free(next_abs_path);
					return ERROR;
				}
				handle_directories(pwd, next_path, options, put_dir_name);
				free(next_path);
			}
			free(next_abs_path);
		}
	}
	free(st);
	return SUCCESS;
}

void	handle_directories(char *pwd, char *path, unsigned char options, int put_dir_name)
{
	t_dir_info	*dir_info;
	char		*abs_path;

	abs_path = get_abs_path(pwd, path, options, put_dir_name);
	if (!abs_path)
		return ;
	dir_info = get_dir_info(path, abs_path, options);
	if (!dir_info) {
		free(abs_path);
		return ;
	}
	print_content(dir_info, path, options);
	if (RECURSIVE_OPT(options) && dir_info->is_dir) {
		if (launch_recursion(dir_info, pwd, abs_path, path, options, put_dir_name) == ERROR) {
			for (int i = 0; i < dir_info->size; i++)
				free(dir_info->content[i]);
			free(dir_info->content);
			free(dir_info);
			free(abs_path);
			return ;
		}
	}
	for (int i = 0; i < dir_info->size; i++)
		free(dir_info->content[i]);
	free(dir_info->content);
	free(dir_info);
	free(abs_path);
}
