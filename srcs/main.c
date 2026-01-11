/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:58:50 by romain            #+#    #+#             */
/*   Updated: 2026/01/11 11:58:24 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	invalid_argument_message(char arg)
{
	ft_putstr_fd(RED "Invalid argument: ", 2);
	ft_putchar_fd(arg, 2);
	ft_putstr_fd("\n" RESET, 2);
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

static char	*find_current_directory(char **env)
{
	for (int i = 0; env[i]; i++)
		if (!ft_strncmp(env[i], "PWD=", 4))
			return (env[i] + 4);
	return NULL;
}

int	main(int ac, char **av, char **env)
{
	unsigned char	options = 0;
	int				directories_count = 0;
	int				files_count = 0;

	for (int i = 1; i < ac; i++) {
		if (av[i][0] == '-') {
			if (!av[i][1]) {
				invalid_argument_message(127);
				return ERROR;
			}
			for (int j = 1; av[i][j]; j++) {
				options = add_option(options, av[i][j]);
				if (options == 255)
					return ERROR;
			}
		} else {
			if (is_dir(av[i]) == FALSE) {
				files_count++;
			} else {
				directories_count++;
			}
		}
	}
	char	*pwd = find_current_directory(env);
	if (!pwd) {
		ft_putstr_fd(RED "Error: can't find PWD in env\n" RESET, 2);
		return ERROR;
	}
	if (directories_count + files_count == 0)
		handle_directories(pwd, ".", options, 0);
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
				if (status == TRUE && directories_count > 0)
					directories[j++] = av[i];
				else if (status == FALSE && files_count > 0)
					files[k++] = av[i];
				else if (status == ERROR) {
					if (directories_count > 0)
						free(directories);
					if (files_count > 0)
						free(files);
					return ERROR;
				}
			}
		}
		if (files_count > 0) {
			files[k] = NULL;
			handle_files(pwd, files, files_count, options);
			free(files);
		}
		if (directories_count > 0) {
			directories[j] = NULL;
			for (int i = 0; i < directories_count; i++) {
				if (directories[i][0] == '/')
					handle_directories(directories[i], ".", options, directories_count > 1);
				else
					handle_directories(pwd, directories[i], options, directories_count > 1);
			}
			free(directories);
		}
	}
}
