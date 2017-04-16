/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissa-b <maissa-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/01 17:16:24 by nbelouni          #+#    #+#             */
/*   Updated: 2017/04/15 15:14:20 by alallema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void	launch_job(t_job *j);
void	export_job(t_tree *root, t_list **g_job_list);
void	printJobList(t_list *g_job_list);

t_list	*g_job_list = NULL;
t_core	*g_core;
t_job	*last_job = NULL;

static t_builtin_array g_builtin_array[] =
{
	{"cd", &ft_builtin_cd},
	{"env", &ft_builtin_env},
	{"setenv", &ft_builtin_setenv},
	{"unsetenv", &ft_builtin_unsetenv},
	{"exit", &ft_builtin_exit},
	{"echo", &ft_builtin_echo},
	{"history", &ft_builtin_history},
	{"unset", &ft_builtin_unset},
	{"export", &ft_builtin_export},
	{NULL, NULL}
};

int		parse_builtins(t_core *core, char *cmd, char **cmd_args)
{
	int	i;

	i = -1;
	while (g_builtin_array[++i].cmd)
	{
		if (ft_strcmp(g_builtin_array[i].cmd, cmd) == 0)
			return (g_builtin_array[i].func(core, cmd_args));
	}
	return (1);
}

/*
**	  init_core initialisation des liste d'env
*/

int		ft_creat_core(char **envp)
{
	if (ft_init_core() != TRUE)
		return (ERR_EXIT);
	g_core->set = ft_init_lstset();
	g_core->exp = ft_init_list();
	if (envp != NULL && envp[0] != NULL)
		g_core->env = ft_env_to_list(envp, g_core->env);
	else
		g_core->env = ft_default_env();
	ft_histopt_r(&(g_core->hist), g_core->set, NULL);
	return (TRUE);
}

/*
**   si l'entree est different du terminal va lire ligne par ligne GNL
*/

int 	main(int argc, char **argv, char **envp)
{
	(void)argc;
  	(void)argv;
	t_completion	completion = {NULL, NULL, NULL, NULL};
	t_buf	*buf;
	t_token	*list;
	int		ret;
	int		ret_read;
	t_tree	*ast;
	t_list	*job_list_bis = NULL;
	struct termios termio;

	tcgetattr(0, &termio);
	ast = NULL;
	list = NULL;
	if (ft_creat_core(envp) == ERR_EXIT)
		return (ERR_EXIT);
	if (!(buf = init_buf()))
		return (ft_print_error("21sh", ERR_MALLOC, ERR_EXIT));
	if (init_completion(&completion, g_core) == ERR_EXIT)
		return (-1);
	if (!isatty(0))
		return (ft_print_error("21sh", " : Input is not from a terminal", ERR_EXIT));
	set_prompt(PROMPT1, ft_strlen(PROMPT1));
	init_shell();
	g_core->buf = buf;
	init_curs();
	while ((ret_read = read_line(g_core->buf, &completion, g_core->hist)) != ERR_EXIT)
	{
		close_termios();
		job_list_bis = NULL;
		if (ret_read != TAB)
		{
			if (is_line_ended(g_core->buf) < 0)
				return (-1);
			bang_substitution(&(g_core->buf->final_line), g_core);
			ret = parse_buf(&list, g_core->buf->final_line, &completion, g_core->hist);
			if (ret > 0 && list)
			{
				if ((ret = ft_cmd_to_history(g_core->hist, buf->final_line)) == ERR_EXIT)
					return (ft_print_error("21sh: ", ERR_MALLOC, ERR_EXIT));
				ft_push_ast(list, &ast);
				print_t(ast);
				export_job(ast, &job_list_bis);
				list_iter(job_list_bis, (void *)launch_job);
				delete_list(&job_list_bis, NULL);
				free_ast(ast);
				if ((ret = ft_check_history_var(g_core->set, g_core->hist)) == ERR_EXIT)
					return (ft_print_error("21sh: ", ERR_MALLOC, ERR_EXIT));
			}
			if (ret != ERR_NEW_PROMPT && g_core->buf->final_line)
				ft_strdel(&(g_core->buf->final_line));
			else
				complete_final_line(g_core->buf, list);
			if (list)
				ft_tokendestroy(&list); //clean la list a mettre a la fin
			ft_bzero(g_core->buf->line, BUFF_SIZE);
			g_core->buf->size = 0;
			clean_pos_curs();
			if (init_completion(&completion, g_core) == ERR_EXIT)
				return (-1);
		}
		if (ret_read == END_EOT)
			break ;
	}
	close_termios();
	free_buf(g_core->buf);
	return (0);
}
