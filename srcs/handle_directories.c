/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directories.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 09:40:02 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 10:51:36 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static char	*get_abs_path(char *pwd, char *path, unsigned char options)
{
	static int	first = 1;
	char		*abs_path;

	if (!first)
		ft_printf("\n");
	else
		first = 0;
	abs_path = malloc(ft_strlen(pwd) + ft_strlen(path) + 2);
	if (!abs_path)
		return (NULL);
	ft_strlcpy(abs_path, pwd, ft_strlen(pwd) + 1);
	abs_path[ft_strlen(pwd)] = '/';
	ft_strlcpy(abs_path + ft_strlen(pwd) + 1, path, ft_strlen(path) + 1);
	if (RECURSIVE_OPT(options))
		ft_printf("%s:\n", path);
	return (abs_path);
}

static char	*get_next_abs_path(char *abs_path, char *dir)
{
	char	*next_abs_path;

	next_abs_path = malloc(ft_strlen(abs_path) + ft_strlen(dir) + 2);
	if (!next_abs_path)
		return (NULL);
	ft_strlcpy(next_abs_path, abs_path, ft_strlen(abs_path) + 1);
	next_abs_path[ft_strlen(abs_path)] = '/';
	ft_strlcpy(next_abs_path + ft_strlen(abs_path) + 1, dir,
		ft_strlen(dir) + 1);
	return (next_abs_path);
}

static char	*get_next_path(char *path, char *dir)
{
	char	*next_path;

	next_path = malloc(ft_strlen(path) + ft_strlen(dir) + 2);
	if (!next_path)
		return (NULL);
	ft_strlcpy(next_path, path, ft_strlen(path) + 1);
	next_path[ft_strlen(path)] = '/';
	ft_strlcpy(next_path + ft_strlen(path) + 1, dir, ft_strlen(dir) + 1);
	return (next_path);
}

static int	launch_recursion(t_dir_info *dir_info, char *pwd,
	char *abs_path, char *path, unsigned char options)
{
	int			i;
	struct stat	*st;
	char		*next_abs_path;
	char		*next_path;

	st = malloc(sizeof(struct stat));
	if (!st)
		return (ERROR);
	i = -1;
	while (++i < dir_info->size)
	{
		if (dir_info->content[i]->d_name[0] != '.' || ALL_OPT(options))
		{
			next_abs_path = get_next_abs_path(abs_path,
					dir_info->content[i]->d_name);
			if (!next_abs_path)
				return (free(st), ERROR);
			stat(next_abs_path, st);
			if (S_ISDIR(st->st_mode)
				&& ft_strcmp(dir_info->content[i]->d_name, ".")
				&& ft_strcmp(dir_info->content[i]->d_name, ".."))
			{
				next_path = get_next_path(path, dir_info->content[i]->d_name);
				if (!next_path)
					return (free(st), free(next_abs_path), ERROR);
				handle_directories(pwd, next_path, options);
				free(next_path);
			}
			free(next_abs_path);
		}
	}
	return (free(st), SUCCESS);
}

void	handle_directories(char *pwd, char *path, unsigned char options)
{
	t_dir_info		*dir_info;
	char			*abs_path;
	// int				i;

	abs_path = get_abs_path(pwd, path, options);
	if (!abs_path)
		exit(ERROR);
	dir_info = get_dir_info(abs_path);
	if (!dir_info)
	{
		free(abs_path);
		exit(ERROR);
	}
	// i = -1;
	// while (++i < dir_info->size)
	// {
	// 	if (dir_info->content[i]->d_name[0] != '.' || ALL_OPT(options))
	// 	{
	// 		if (LIST_OPT(options))
	// 		{
	// 			if (print_on_list_format(path, dir_info->content[i]) == ERROR)
	// 				(free(dir_info->content), free(dir_info), free(abs_path));
	// 		}
	// 		else
	// 			ft_printf("%s\n", dir_info->content[i]->d_name);
	// 	}
	// }
	print_content(dir_info, path, options);
	if (RECURSIVE_OPT(options))
	{
		if (launch_recursion(dir_info, pwd, abs_path, path, options) == ERROR)
		{
			closedir(dir_info->dir);
			(free(dir_info->content), free(dir_info), free(abs_path));
			exit(ERROR);
		}
	}
	closedir(dir_info->dir);
	(free(dir_info->content), free(dir_info), free(abs_path));
}
