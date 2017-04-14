/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissa-b <maissa-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 18:15:50 by nbelouni          #+#    #+#             */
/*   Updated: 2017/04/15 00:14:28 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

int		count_prev_char(char *s, int i, char c)
{
	int len;

	len = 0;
	while (i - len >= 0 && s[i - len] == c)
	{
		len += 1;
	}
	return (len);
}

int		is_char(char *s, int i, char c)
{
	if (i == 0 || count_prev_char(s, i - 1, '\\') % 2 == 0)
	{
		if (s[i] == c)
			return (1);
	}
	return (0);
}

int		find_prev_char(char *s, int len, char c)
{
	int		i;

	i = 0;
	while (len - i > 0)
	{
		if (is_char(s, len - i, c))
			return (i);
		i++;
	}
	return (0);
}

int		find_next_char(char *s, int len, char c)
{
	int		i;

	i = 0;
	while (len + i < (int)ft_strlen(s))
	{
		if (is_char(s, len + i, c))
			return (i);
		i++;
	}
	return (-1);
}

int		is_cmd(char *s, int i)
{
	while (i >= 0 && (is_char(s, i, ' ') || is_any_quote(s, i)))
		i--;
	if (i < 0)
		return (1);
	if (i >= 0 &&
	(is_separator(s, i) || is_char(s, i, '(') || is_char(s, i, '{')))
		return (1);
	return (0);
}

int		is_arg(char *s, int i)
{
	if (i < 0)
		return (1);
	while (i >= 0 && is_char(s, i, ' '))
		i--;
	if (i >= 0 && !is_separator(s, i))
		return (1);
	return (0);
}
