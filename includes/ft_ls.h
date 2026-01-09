/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:59:59 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 10:52:55 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "../libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <strings.h>
# include <ctype.h>
# include <dirent.h>
# include <time.h>
# include <pwd.h>
# include <grp.h>
# include <sys/types.h>
# include <sys/stat.h>

# define RESET		"\033[0m"
# define RED		"\033[31;1m"
# define GREEN		"\033[32;1m"
# define YELLOW		"\033[33;1m"
# define BLUE		"\033[34;1m"
# define MAGENTA	"\033[35;1m"
# define CYAN		"\033[36;1m"

# define SUCCESS	0
# define ERROR		1

# define       ALL_OPT(opt) opt >> 0 & 1U
# define      LIST_OPT(opt) opt >> 1 & 1U
# define      TIME_OPT(opt) opt >> 2 & 1U
# define   REVERSE_OPT(opt) opt >> 3 & 1U
# define RECURSIVE_OPT(opt) opt >> 4 & 1U

typedef struct s_dir_info
{
	DIR				*dir;
	int				size;
	struct dirent	**content;
}	t_dir_info;

int			ft_strcasecmp(const char *s1, const char *s2);
t_dir_info	*get_dir_info(char *path);
void		print_content(t_dir_info *dir_info, char *path, unsigned char options);
char		**get_content_list(t_dir_info *dir_info, char *path, unsigned char options);
void		handle_directories(char *pwd, char *path, unsigned char options);

#endif
