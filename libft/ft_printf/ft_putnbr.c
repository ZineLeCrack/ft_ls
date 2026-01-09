/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:41:15 by rlebaill          #+#    #+#             */
/*   Updated: 2026/01/08 10:13:37 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_itoalen(long n, int len)
{
	long	coeff;

	if (n < 0)
	{
		len++;
		n *= -1;
	}
	coeff = 1;
	if (n == 0)
		len++;
	while (coeff <= n)
	{
		len++;
		coeff *= 10;
	}
	return (len);
}

void	ft_putnbr(int n)
{
	char	tmp;

	if (n < -2100000000)
	{
		write(1, "-2", 2);
		ft_putnbr(-(n + 2000000000));
	}
	else if (n < 0)
	{
		write(1, "-", 1);
		ft_putnbr(-n);
	}
	else if (0 <= n && n <= 9)
	{
		tmp = n + '0';
		write(1, &tmp, 1);
	}
	else
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}
}

int	ft_putnbr_printf(int n)
{
	int	len;

	len = 0;
	len = ft_itoalen(n, len);
	ft_putnbr(n);
	return (len);
}
