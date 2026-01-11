/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:58:50 by romain            #+#    #+#             */
/*   Updated: 2026/01/11 18:05:01 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	invalid_argument_message(char arg)
{
	ft_putstr_fd(RED "ft_ls: invalid option -- '", 2);
	ft_putchar_fd(arg, 2);
	ft_putstr_fd("'\n" RESET, 2);
}

static unsigned char	add_option(unsigned char options, char option)
{
	switch (option)
	{
		case 'a':
			return (options | (unsigned char)0b00000001);	
		case 'l':
			return (options | (unsigned char)0b00000010);
		case 't':
			return (options | (unsigned char)0b00000100);
		case 'r':
			return (options | (unsigned char)0b00001000);
		case 'R':
			return (options | (unsigned char)0b00010000);
		default:
		{
			invalid_argument_message(option);
			return 255;
		}
	}
}

int	main(int ac, char **av)
{
	unsigned char	options = 0;
	int				directories_count = 0;
	int				files_count = 0;
	int				exit_status = 0;

	for (int i = 1; i < ac; i++) {
		if (av[i][0] == '-') {
			if (!av[i][1]) {
				invalid_argument_message(127);
				return 2;
			}
			for (int j = 1; av[i][j]; j++) {
				options = add_option(options, av[i][j]);
				if (options == 255)
					return 2;
			}
		} else {
			int	status = is_dir(av[i]);
			if (status == TRUE) {
				directories_count++;
			} else if (status == FALSE) {
				files_count++;
			} else if (status == NSFOD) {
				ft_putstr_fd(RED "ft_ls: cannot access '", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("': No such file or directory\n" RESET, 2);
				exit_status = 2;
			} else if (status == NO_PERM) {
				ft_putstr_fd(RED "ft_ls: cannot open directory '", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("': Permission denied\n" RESET, 2);
				exit_status = 2;
			}
		}
	}
	if (directories_count + files_count == 0 && exit_status == 0)
		handle_directories(".", options, 0);
	else {
		char	**directories = NULL;
		if (directories_count > 0) {
			directories = malloc(sizeof(char *) * (directories_count + 1));
			if (!directories) {
				ft_putstr_fd(RED "Fatal error\n" RESET, 2);
				return ERROR;
			}
		}
		char	**files = NULL;
		if (files_count > 0) {
			files = malloc(sizeof(char *) * (files_count + 1));
			if (!files) {
				ft_putstr_fd(RED "Fatal error\n" RESET, 2);
				if (directories_count > 0)
					free(directories);
				return ERROR;
			}
		}
		int		j = 0, k = 0;
		for (int i = 1; i < ac; i++) {
			if (av[i][0] != '-') {
				int status = is_dir(av[i]);
				if (status == ERROR) {
					if (directories_count > 0)
						free(directories);
					if (files_count > 0)
						free(files);
					return ERROR;
				}
				if (status == TRUE && directories_count > 0)
					directories[j++] = av[i];
				else if (status == FALSE && files_count > 0)
					files[k++] = av[i];
			}
		}
		if (files_count > 0) {
			files[k] = NULL;
			handle_files(files, files_count, options);
			free(files);
		}
		if (directories_count > 0) {
			directories[j] = NULL;
			sort_content(directories_count, &directories, ".", options);
			if (REVERSE_OPT(options))
				reverse_content(directories_count, &directories);
			for (int i = 0; i < directories_count; i++) {
				handle_directories(directories[i], options, directories_count + (files_count > 0) > 1);
			}
			free(directories);
		}
	}
	return exit_status;
}
