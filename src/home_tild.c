/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   home_tild.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/21 22:32:29 by nbelouni          #+#    #+#             */
/*   Updated: 2017/04/21 22:33:51 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

extern t_core	*g_core;

void	home_tild(t_buf *buf, int *begin)
{
	char	*tmp;
	int		len;
	int		c_pos;

	if (ft_find_elem("HOME", g_core->env) != NULL)
	{
		len = 0;
		c_pos = (g_curs.win_col * g_curs.row + g_curs.col) - get_prompt_len();
		while (c_pos > 0 && buf->line[c_pos - 1] != '~')
		{
			len += 1;
			m_left(1);
			c_pos -= 1;
		}
		vb_del(buf, DEL);
		tmp = buf->to_paste;
		buf->to_paste = ft_strdup(ft_find_elem("HOME", g_core->env)->value);
		buf->to_paste_size = ft_strlen(buf->to_paste);
		vb_paste(buf);
		m_right(len + buf->to_paste_size);
		ft_strdel(&(buf->to_paste));
		buf->to_paste = tmp;
		buf->to_paste_size = ft_strlen(tmp);
		*begin = find_word_begin(buf->line, c_pos);
	}
}
