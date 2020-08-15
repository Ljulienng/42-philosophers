/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 18:47:07 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/02 18:47:11 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static char	*ft_itoa_bis(int count, int sign, int div, long nb)
{
	char	*s;
	long	res;
	int		i;

	res = 0;
	i = 0;
	if (!(s = malloc(sizeof(char) * (count + 1 + sign + 1))))
		return (NULL);
	if (sign == 1)
		s[i++] = '-';
	while (div > 0)
	{
		res = (nb / div) % 10;
		s[i++] = res + 48;
		div = div / 10;
	}
	s[i] = '\0';
	return (s);
}

char		*ft_itoa(long n)
{
	int		div;
	int		count;
	int		sign;
	long	nb;

	div = 1;
	sign = 0;
	count = 0;
	nb = (long)n;
	if (nb < 0)
	{
		sign++;
		nb *= -1;
	}
	while (nb / div >= 10)
	{
		div = div * 10;
		count++;
	}
	return (ft_itoa_bis(count, sign, div, nb));
}


// static int	ft_nbrlen_bonus(long int nbr)
// {
// 	int	nbrlen;

// 	nbrlen = 1;
// 	if (nbr < 0)
// 		nbr = -nbr;
// 	while (nbr >= 10)
// 	{
// 		nbr /= 10;
// 		nbrlen = nbrlen + 1;
// 	}
// 	return (nbrlen);
// }

// char		*ft_itoa(int n)
// {
// 	int		nbrlen;
// 	char	*str;
// 	long	n2;

// 	n2 = (long int)n;
// 	nbrlen = ft_nbrlen_bonus(n2);
// 	if (n < 0)
// 	{
// 		n2 = -n2;
// 		nbrlen++;
// 	}
// 	if (!(str = (char*)malloc((nbrlen + 1) * sizeof(char))))
// 		return (0);
// 	str[0] = '-';
// 	str[nbrlen--] = '\0';
// 	if (n2 == 0)
// 		str[nbrlen--] = '0';
// 	while (n2)
// 	{
// 		str[nbrlen--] = n2 % 10 + '0';
// 		n2 = n2 / 10;
// 	}
// 	return (str);
// }