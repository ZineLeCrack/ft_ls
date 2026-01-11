/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:49:05 by romain            #+#    #+#             */
/*   Updated: 2026/01/11 14:06:34 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static char	get_type(mode_t mode)
{
	if (S_ISREG(mode))
		return '-';
	else if (S_ISDIR(mode))
		return 'd';
	else if (S_ISLNK(mode))
		return 'l';
	else if (S_ISCHR(mode))
		return 'c';
	else if (S_ISBLK(mode))
		return 'b';
	else if (S_ISFIFO(mode))
		return 'p';
	else
		return '-';
}

static struct stat	**malloc_stat_list(t_dir_info *dir_info, unsigned char options, int *size)
{
	struct stat	**st;
	int			count;

	count = 0;
	for (int i = 0; i < dir_info->size; i++) {
		if (!dir_info->is_dir || dir_info->content[i][0] != '.' || ALL_OPT(options))
			count++;
	}
	st = malloc(sizeof(struct stat *) * (count + 1));
	if (!st) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return NULL;
	}
	int i;
	for (i = 0; i < count; i++) {
		st[i] = malloc(sizeof(struct stat));
		if (!st[i]) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			for (int j = 0; j < i; j++)
				free(st[i]);
			free(st);
			return NULL;
		}
	}
	st[i] = NULL;
	(*size) = count;
	return (st);
}

static char	**malloc_content_list(t_dir_info *dir_info, unsigned char options)
{
	int		count = 0;
	char	**content_list;

	for (int i = 0; i < dir_info->size; i++)
		if (!dir_info->is_dir || dir_info->content[i][0] != '.' || ALL_OPT(options))
			count++;
	content_list = malloc(sizeof(char *) * (count + 1));
	if (!content_list) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return NULL;
	}
	return content_list;
}

static int	set_permissions(char **content_list, struct stat **st)
{
	for (int i = 0; st[i]; i++) {
		char	*permission = malloc(sizeof(char) * 12);
		if (!permission) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			for (int j = 0; j < i; j++)
				free(content_list[j]);
			return ERROR;
		}
		permission[0] = get_type(st[i]->st_mode);
		permission[1] = st[i]->st_mode & S_IRUSR ? 'r' : '-';
		permission[2] = st[i]->st_mode & S_IWUSR ? 'w' : '-';
		permission[3] = st[i]->st_mode & S_IXUSR ? 'x' : '-';
		permission[4] = st[i]->st_mode & S_IRGRP ? 'r' : '-';
		permission[5] = st[i]->st_mode & S_IWGRP ? 'w' : '-';
		permission[6] = st[i]->st_mode & S_IXGRP ? 'x' : '-';
		permission[7] = st[i]->st_mode & S_IROTH ? 'r' : '-';
		permission[8] = st[i]->st_mode & S_IWOTH ? 'w' : '-';
		permission[9] = st[i]->st_mode & S_IXOTH ? 'x' : '-';
		permission[10] = ' ';
		permission[11] = '\0';
		content_list[i] = permission;
	}
	return SUCCESS;
}

static char	*get_same_len_join(char *s1, char *s2, int size, char c)
{
	char	*str = malloc(sizeof(char) * (size + 1));
	if (!str) {
		ft_putstr_fd(RED "Fatal error\n" RESET, 2);
		return NULL;
	}

	int	i;
	for (i = 0; s1[i]; i++)
		str[i] = s1[i];
	for (size_t j = 0; j < size - (ft_strlen(s1) + ft_strlen(s2)); j++)
		str[i++] = c;
	for (int j = 0; s2[j]; j++)
		str[i++] = s2[j];
	str[i] = '\0';
	return str;
}

static int	set_nlink(char **content_list, struct stat **st, int size)
{
	char	*numbers[size];
	int		max = 0;

	for (int i = 0; st[i]; i++) {
		char	*n = ft_itoa((int)(st[i]->st_nlink));
		if (!n) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			for (int j = 0; j < i; j++)
				free(numbers[j]);
			return ERROR;
		}
		int		len = ft_strlen(n);
		numbers[i] = n;
		max = len > max ? len : max;
	}
	for (int i = 0; st[i]; i++) {
		char	*tmp = get_same_len_join(content_list[i], numbers[i], max + ft_strlen(content_list[i]), ' ');
		if (!tmp) {
			for (int j = i; j < size; j++)
				free(numbers[j]);
			return ERROR;
		}
		free(content_list[i]);
		free(numbers[i]);
		content_list[i] = tmp;
	}
	return SUCCESS;
}

static int	set_user_name(char **content_list, struct stat **st, int size)
{
	char	*names[size];
	int		max = 0;

	for (int i = 0; i < size; i++) {
		names[i] = getpwuid(st[i]->st_uid)->pw_name;
		int	len = ft_strlen(names[i]);
		max = len > max ? len : max;
	}
	for (int i = 0; i < size; i++) {
		char	*tmp = get_same_len_join(content_list[i], names[i], max + ft_strlen(content_list[i]) + 1, ' ');
		if (!tmp)
			return ERROR;
		free(content_list[i]);
		content_list[i] = tmp;
	}
	return SUCCESS;
}

static int	set_group_name(char **content_list, struct stat **st, int size)
{
	char	*names[size];
	int		max = 0;

	for (int i = 0; i < size; i++) {
		names[i] = getgrgid(st[i]->st_gid)->gr_name;
		int	len = ft_strlen(names[i]);
		max = len > max ? len : max;
	}
	for (int i = 0; i < size; i++) {
		char	*tmp = get_same_len_join(content_list[i], names[i], max + ft_strlen(content_list[i]) + 1, ' ');
		if (!tmp)
			return ERROR;
		free(content_list[i]);
		content_list[i] = tmp;
	}
	return SUCCESS;
}

static int	set_size(char **content_list, struct stat **st, int size)
{
	char	*sizes[size];
	int		max = 0;

	for (int i = 0; st[i]; i++) {
		char	*n = ft_itoa((int)(st[i]->st_size));
		if (!n) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			for (int j = 0; j < i; j++)
				free(sizes[j]);
			return ERROR;
		}
		int		len = ft_strlen(n);
		sizes[i] = n;
		max = len > max ? len : max;
	}
	for (int i = 0; st[i]; i++) {
		char	*tmp = get_same_len_join(content_list[i], sizes[i], max + ft_strlen(content_list[i]) + 1, ' ');
		if (!tmp) {
			for (int j = i; j < size; j++)
				free(sizes[j]);
			return ERROR;
		}
		free(content_list[i]);
		free(sizes[i]);
		content_list[i] = tmp;
	}
	return SUCCESS;
}

static int	set_time(char **content_list, struct stat **st, int size)
{
	char	*times[size];
	int		max = 0;

	for (int i = 0; st[i]; i++) {
		char	*time = ft_substr(ctime(&st[i]->st_mtime), 4, 12);
		if (!time) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			for (int j = 0; j < i; j++)
				free(times[j]);
			return ERROR;
		}
		int		len = ft_strlen(time);
		times[i] = time;
		max = len > max ? len : max;
	}
	for (int i = 0; st[i]; i++) {
		char	*tmp = get_same_len_join(content_list[i], times[i], max + ft_strlen(content_list[i]) + 1, ' ');
		if (!tmp) {
			for (int j = i; j < size; j++)
				free(times[j]);
			return ERROR;
		}
		free(content_list[i]);
		free(times[i]);
		content_list[i] = tmp;
	}
	return SUCCESS;
}

static int	set_name(char **content_list, char *names[], int size)
{
	for (int i = 0; i < size; i++) {
		char	*tmp = get_same_len_join(content_list[i], names[i], ft_strlen(content_list[i]) + ft_strlen(names[i]) + 1, ' ');
		if (!tmp)
			return ERROR;
		free(content_list[i]);
		content_list[i] = tmp;
	}
	return SUCCESS;
}

char	**get_content_list(t_dir_info *dir_info, char *path, unsigned char options)
{
	int			size;
	struct stat	**st = malloc_stat_list(dir_info, options, &size);
	if (!st)
		return NULL;

	char	**content_list = malloc_content_list(dir_info, options);
	if (!content_list) {
		for (int i = 0; st[i]; i++)
			free(st[i]);
		free(st);
		return NULL;
	}

	char	*names[size];

	blkcnt_t	total_blocks = 0;
	content_list[size] = NULL;
	int	j = 0;
	for  (int i = 0; i < dir_info->size; i++) {
		if (!dir_info->is_dir || dir_info->content[i][0] != '.' || ALL_OPT(options)) {
			names[j] = dir_info->content[i];
			char	*next_path = get_next_path(path, dir_info->content[i]);
			if (!next_path) {
				for (int i = 0; st[i]; i++)
					free(st[i]);
				free(st);
				free(content_list);
				return NULL;
			}
			stat(next_path, st[j]);
			total_blocks += st[j++]->st_blocks;
			free(next_path);
		}
	}
	if (dir_info->is_dir)
		ft_printf("total %i\n", total_blocks >> 1);
	if (set_permissions(content_list, st) == ERROR) {
		for (int i = 0; st[i]; i++)
			free(st[i]);
		free(st);
		free(content_list);
		return NULL;
	}
	if (
		set_nlink     (content_list,    st, size)	== ERROR ||
		set_user_name (content_list,    st, size)	== ERROR ||
		set_group_name(content_list,    st, size)	== ERROR ||
		set_size      (content_list,    st, size)	== ERROR ||
		set_time      (content_list,    st, size)	== ERROR ||
		set_name      (content_list, names, size)	== ERROR
	) {
		for (int i = 0; st[i]; i++)
			free(st[i]);
		free(st);
		for (int i = 0; content_list[i]; i++)
			free(content_list[i]);
		free(content_list);
		return NULL;
	}
	for (int k = 0; st[k]; k++)
		free(st[k]);
	free(st);
	return (content_list);
}
