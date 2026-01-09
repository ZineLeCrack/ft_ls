/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_content.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 10:35:00 by romain            #+#    #+#             */
/*   Updated: 2026/01/09 15:22:53 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_content(t_dir_info *dir_info, char *path, unsigned char options)
{
	char	**content_list;

	if (LIST_OPT(options)) {
		content_list = get_content_list(dir_info, path, options);
		if (!content_list)
			return ;
		for (int i = 0; content_list[i]; i++) {
			ft_printf("%s\n", content_list[i]);
			free(content_list[i]);
		}
		free(content_list);
	} else {
		for (int i = 0; i < dir_info->size; i++) {
			if (dir_info->content[i]->d_name[0] != '.' || ALL_OPT(options))
				ft_printf("%s\n", dir_info->content[i]->d_name);
		}
	}
}
