/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_message.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 12:09:13 by romain            #+#    #+#             */
/*   Updated: 2026/01/27 12:20:26 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	display_help_message()
{
	ft_printf("\
Usage: ls [OPTION]... [FILE]...\n\
List information about the FILEs (the current directory by default).\n\
Sort entries alphabetically if -t is not specified.\n\
\n\
  -a                         do not ignore entries starting with .\n\
  -l                         use a long listing format\n\
  -r                         reverse order while sorting\n\
  -R                         list subdirectories recursively\n\
  -t                         sort by time, newest first\n\
      --help        display this help and exit\n\
");
	exit(0);
}
