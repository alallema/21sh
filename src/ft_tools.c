/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissa-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/22 17:54:58 by maissa-b          #+#    #+#             */
/*   Updated: 2017/04/25 23:47:16 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

extern t_core *g_core;

int		ft_creat_core(char **envp)
{
	t_elem	*elem;

	if (ft_init_core() != TRUE)
		return (ERR_EXIT);
	g_core->set = ft_init_lstset();
	g_core->exp = ft_init_list();
	if (envp != NULL && envp[0] != NULL)
	{
		g_core->env = ft_env_to_list(envp, g_core->env);
		elem = NULL;
		if ((elem = ft_find_elem("SHLVL", g_core->env)) != NULL)
			up_shlvl(&elem);
		else
			ft_setenv(g_core->env, "SHLVL", "2");
	}
	else
		g_core->env = ft_default_env();
	ft_histopt_r(g_core, NULL);
	return (TRUE);
}

int		up_shlvl(t_elem **elem)
{
	int		lvl;

	if ((*elem)->value != NULL && (*elem)->value[0] != '\0')
	{
		lvl = ft_atoi((*elem)->value);
		++lvl;
	}
	else
	{
		lvl = 2;
	}
	ft_strdel(&((*elem)->value));
	if (((*elem)->value = ft_itoa(lvl)) == NULL)
		return (ERR_EXIT);
	return (0);
}

int		init_begin_end(char *s, int *begin, int *end)
{
	int		i;

	*begin = find_next_char(s, 0, '$');
	if (*begin < 0 || !ft_isalpha(s[*begin + 1]) ||
	which_quotes(s, *begin) == S_QUOTE)
		return (TRUE);
	*begin += 1;
	i = *begin;
	*end = 0;
	while (s[i + *end] && ft_isalnum(s[i + *end]))
		*end += 1;
	return (FALSE);
}

void	init_line(t_buf *buf)
{
	init_termios();
	ft_putstr_fd(get_prompt_str(), 1);
	print_pre_curs(buf);
	print_post_curs(buf);
}

/*
**  integre les copier coller a la souris
*/

int		classic_read(t_buf *buf, int x)
{
	int tmp;

	tmp = x & 0xff;
	if (x == 25)
		return (0);
	if (tmp < 31 || tmp > 127)
		return (1);
	while (x)
	{
		tmp = x & 0xff;
		x >>= 8;
		read_modul(tmp, buf);
	}
	return (1);
}
