/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directories.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 09:40:02 by romain            #+#    #+#             */
/*   Updated: 2026/01/28 17:30:49 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	launch_recursion(t_dir_info *dir_info, char *path, unsigned char options, int put_dir_name)
{
	struct stat	*st;
	char		*next_path;

	st = malloc(sizeof(struct stat));
	if (!st) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return ERROR;
	}
	for (int i = 0; i < dir_info->size; i++) {
		if (dir_info->content[i][0] != '.' || ALL_OPT(options)) {
			next_path = get_next_path(path, dir_info->content[i]);
			if (!next_path) {
				free(st);
				return ERROR;
			}
			lstat(next_path, st);
			if (S_ISDIR(st->st_mode)
			&& ft_strcmp(dir_info->content[i], ".")
			&& ft_strcmp(dir_info->content[i], ".."))
			{
				handle_directories(next_path, options, put_dir_name);
			}
			free(next_path);
		}
	}
	free(st);
	return SUCCESS;
}

void	handle_directories(char *path, unsigned char options, int put_dir_name)
{
	t_dir_info	*dir_info;

	dir_info = get_dir_info(path, options);
	if (!dir_info) {
		return ;
	}
	print_content(dir_info, path, put_dir_name, options);
	if (RECURSIVE_OPT(options) && dir_info->is_dir) {
		if (launch_recursion(dir_info, path, options, put_dir_name) == ERROR) {
			for (int i = 0; i < dir_info->size; i++)
				free(dir_info->content[i]);
			free(dir_info->content);
			free(dir_info);
			return ;
		}
	}
	for (int i = 0; i < dir_info->size; i++)
		free(dir_info->content[i]);
	free(dir_info->content);
	free(dir_info);
}

void	handle_files(char **files, int size, unsigned char options)
{
	t_dir_info	*dir_info = malloc(sizeof(t_dir_info));
	if (!dir_info) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return ;
	}

	sort_content(size, &files, ".", options);
	if (REVERSE_OPT(options)) {
		reverse_content(size, &files);
	}
	dir_info->content = files;
	dir_info->size = size;
	dir_info->is_dir = 0;
	print_content(dir_info, ".", 0, options);
	free(dir_info);
}
