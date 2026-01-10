/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:58:50 by romain            #+#    #+#             */
/*   Updated: 2026/01/10 12:58:07 by romain           ###   ########.fr       */
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
	int				count = 0;

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
			count++;
		}
	}
	char	*pwd = find_current_directory(env);
	if (!pwd) {
		ft_putstr_fd(RED "Error: can't find PWD in env\n" RESET, 2);
		return ERROR;
	}
	if (count > 0) {
		char	**args = malloc(sizeof(char *) * (count + 1));
		if (!args) {
			ft_putstr_fd(RED "Fatal error\n" RESET, 2);
			return ERROR;
		}
		int		j = 0;
		for (int i = 1; i < ac; i++) {
			if (av[i][0] != '-') {
				args[j++] = av[i];
			}
		}
		args[j] = NULL;
		for (int i = 0; i < count; i++) {
			if (args[i][0] == '/')
				handle_directories(args[i], ".", options, count > 1);
			else
				handle_directories(pwd, args[i], options, count > 1);
		}
		free(args);
	} else {
		handle_directories(pwd, ".", options, 0);
	}
}
