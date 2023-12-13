/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:19:16 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 12:28:33 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*res;
	int		i;

	res = mmalloc(sizeof(char) * ft_strlen(s) + 1);
	if (!(res))
		return (NULL);
	i = 0;
	while (s[i])
	{
		res[i] = (*f)((unsigned int)i, s[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}
