/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:49:05 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 15:12:24 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static char	*get_abs_path(char *path, char *name)
{
	char		*abs_path;

	abs_path = malloc(ft_strlen(path) + ft_strlen(name) + 2);
	if (!abs_path)
		return (NULL);
	ft_strlcpy(abs_path, path, ft_strlen(path) + 1);
	abs_path[ft_strlen(path)] = '/';
	ft_strlcpy(abs_path + ft_strlen(path) + 1, name, ft_strlen(name) + 1);
	return (abs_path);
}

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

// static void	print_size_and_time(off_t size, time_t time)
// {
// 	char	*str;
// 	long	n;

// 	if (size == 0l)
// 		ft_printf("      ");
// 	else
// 	{
// 		n = 1000000;
// 		while (n > size)
// 		{
// 			n /= 10;
// 			write(1, " ", 1);
// 		}
// 	}
// 	ft_printf("%i ", size);
// 	str = ctime(&time);
// 	write(1, str + 4, 12);
// }

static struct stat	**malloc_stat_list(t_dir_info *dir_info, unsigned char options, int *size)
{
	struct stat	**st;
	int			count;

	count = 0;
	for (int i = 0; i < dir_info->size; i++) {
		if (dir_info->content[i]->d_name[0] != '.' || ALL_OPT(options))
			count++;
	}
	st = malloc(sizeof(struct stat *) * (count + 1));
	int i;
	for (i = 0; i < count; i++)
		st[i] = malloc(sizeof(struct stat));
	st[i] = NULL;
	(*size) = count;
	return (st);
}

static char	**malloc_content_list(t_dir_info *dir_info, unsigned char options)
{
	int	count = 0;

	for (int i = 0; i < dir_info->size; i++)
		if (dir_info->content[i]->d_name[0] != '.' || ALL_OPT(options))
			count++;
	return (malloc(sizeof(char *) * (count + 1)));
}

static void	set_permissions(char **content_list, struct stat **st)
{
	for (int i = 0; st[i]; i++) {
		char	*permission = malloc(sizeof(char) * 12);
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
}

static char	*get_same_len_join(char *s1, char *s2, int size, char c)
{
	char	*str = malloc(sizeof(char) * (size + 1));
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

static void	set_nlink(char **content_list, struct stat **st, int size)
{
	char	*numbers[size];
	int		max = 0;

	for (int i = 0; st[i]; i++) {
		char	*n = ft_itoa((int)(st[i]->st_nlink));
		int		len = ft_strlen(n);
		numbers[i] = n;
		max = len > max ? len : max;
	}
	for (int i = 0; st[i]; i++) {
		char	*tmp = get_same_len_join(content_list[i], numbers[i], max + ft_strlen(content_list[i]), ' ');
		free(content_list[i]);
		free(numbers[i]);
		content_list[i] = tmp;
	}
}

static void	set_user_name(char **content_list, struct stat **st, int size)
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
		free(content_list[i]);
		content_list[i] = tmp;
	}
}

static void	set_group_name(char **content_list, struct stat **st, int size)
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
		free(content_list[i]);
		content_list[i] = tmp;
	}
}

static void	set_size(char **content_list, struct stat **st, int size)
{
	char	*sizes[size];
	int		max = 0;

	for (int i = 0; st[i]; i++) {
		char	*n = ft_itoa((int)(st[i]->st_size));
		int		len = ft_strlen(n);
		sizes[i] = n;
		max = len > max ? len : max;
	}
	for (int i = 0; st[i]; i++) {
		char	*tmp = get_same_len_join(content_list[i], sizes[i], max + ft_strlen(content_list[i]) + 1, ' ');
		free(content_list[i]);
		free(sizes[i]);
		content_list[i] = tmp;
	}
}

static void	set_time(char **content_list, struct stat **st, int size)
{
	char	*times[size];
	int		max = 0;

	for (int i = 0; st[i]; i++) {
		char	*time = ft_substr(ctime(&st[i]->st_mtime), 4, 12);
		int		len = ft_strlen(time);
		times[i] = time;
		max = len > max ? len : max;
	}
	for (int i = 0; st[i]; i++) {
		char	*tmp = get_same_len_join(content_list[i], times[i], max + ft_strlen(content_list[i]) + 1, ' ');
		free(content_list[i]);
		free(times[i]);
		content_list[i] = tmp;
	}
}

static void	set_name(char **content_list, char *names[], int size)
{
	for (int i = 0; i < size; i++) {
		char	*tmp = get_same_len_join(content_list[i], names[i], ft_strlen(content_list[i]) + ft_strlen(names[i]) + 1, ' ');
		free(content_list[i]);
		content_list[i] = tmp;
	}
}

char	**get_content_list(t_dir_info *dir_info, char *path, unsigned char options)
{
	int			size;
	struct stat	**st = malloc_stat_list(dir_info, options, &size);
	char		**content_list = malloc_content_list(dir_info, options);
	char		*names[size];

	content_list[size] = NULL;
	int	j = 0;
	for  (int i = 0; i < dir_info->size; i++)
	{
		if (dir_info->content[i]->d_name[0] != '.' || ALL_OPT(options))
		{
			char	*abs_path = get_abs_path(path, dir_info->content[i]->d_name);
			if (!abs_path)
			return (NULL);
			names[j] = dir_info->content[i]->d_name;
			stat(abs_path, st[j++]);
			free(abs_path);
		}
	}
	set_permissions(content_list, st);
	set_nlink(content_list, st, size);
	set_user_name(content_list, st, size);
	set_group_name(content_list, st, size);
	set_size(content_list, st, size);
	set_time(content_list, st, size);
	set_name(content_list, names, size);
	for (int k = 0; st[k]; k++)
		free(st[k]);
	free(st);
	return (content_list);
}
