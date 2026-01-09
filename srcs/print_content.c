/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_content.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 10:35:00 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 10:53:50 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_content(t_dir_info *dir_info, char *path, unsigned char options)
{
	int		i;
	char	**content_lits;

	if (LIST_OPT(options))
	{
		content_lits = get_content_list(dir_info, path, options);
		if (!content_lits)
			return ;
		i = -1;
		while (content_lits[++i])
			ft_printf("%s\n", content_lits[i]);
	}
	else
	{
		i = -1;
		while (++i < dir_info->size)
		{
			if (dir_info->content[i]->d_name[0] != '.' || ALL_OPT(options))
				ft_printf("%s\n", dir_info->content[i]->d_name);
		}
	}
}
