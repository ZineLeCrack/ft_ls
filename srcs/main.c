/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:58:50 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 11:27:38 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	invalid_argument_message(char arg)
{
	ft_putstr_fd(RED "Invalid argument: ", 2);
	ft_putchar_fd(arg, 2);
	ft_putstr_fd("\n" RESET, 2);
	exit(1);
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
			return 0;
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

	for (int i = 1; i < ac; i++)
	{
		if (av[i][0] == '-')
		{
			if (!av[i][1])
				invalid_argument_message(127);
			while (*(++(av[i])))
				options = add_option(options, *(av[i]));
		}
	}
	char	*pwd = find_current_directory(env);
	if (!pwd) {
		ft_putstr_fd(RED "Error: can't find PWD in env\n" RESET, 2); exit(ERROR);
	}
	handle_directories(pwd, ".", options);
}
