/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouiri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:16:38 by mdouiri           #+#    #+#             */
/*   Updated: 2022/05/07 19:47:17 by mdouiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init(t_stock *data, int ac, char **av, char **env)
{
	data->ac = ac;
	data->av = av;
	data->env = env;
	data->file1 = 0;
	data->file2 = 0;
	data->cut1 = NULL;
	data->cut2 = NULL;
	data->cmd1 = NULL;
	data->cmd2 = NULL;
}

char	*ft_command_exist(char *cmd, char **env)
{
	char	*path;
	char	*path_bis;
	char	*new_path;

	path = (ft_path(env) + 5);
	if(access(cmd, X_OK) == 0)
		return(cmd);
	while(path && (ft_found_char(path, ':') != -1))
	{
		path_bis = ft_strdup(path, ft_found_char(path, ':'));
		new_path = ft_strjoin(path_bis, cmd);
		free(path_bis);
		if(access(new_path, X_OK) == 0)
		{
			return(new_path);
		}
		free(new_path);
		path = path + (ft_found_char(path, ':') + 1);
	}
	return(NULL);
}

void	ft_error(int ac, char **av, char **env)
{
	char	*cmd1;
	char	*cmd2;

	if(ac != 5)
		printf("Invalid nb of args \n");
	if(ft_check_file(av[1], 1) == 1)
		perror(av[1]);
	if(ft_check_file(av[4], 0) == 1)
		perror(av[4]);
	cmd1 = ft_command_exist(av[2], env);
	cmd2 = ft_command_exist(av[3], env);
	if(cmd1 == av[2])
		printf("command not found : %s \n", av[2]);
	if(cmd2 == av[3])
		printf("command not found : %s \n", av[3]);
//	exit(127);

}

void	ft_begin_processes(t_stock *data)
{
	int	fd[2];
	pid_t	pid;
	int	status;
	
	dup2(data->file1, 0);
	dup2(data->file2, 1);
	status = 0;
	pipe(fd);
	pid = fork();
	if(pid == -1)
		return;
	else if(pid == 0)
	{
		dup2(fd[0], 0);
		close(fd[1]);
		close(fd[0]);
		execve(data->cmd2, data->cut2, data->env);
		waitpid(pid, &status , 0);
	}
	else
	{
		close(fd[0]);
		dup2(fd[1], 1);
		execve(data->cmd1, data->cut1, data->env);
		close(fd[1]);
	}
}
#include <stdio.h>

void	ft_parsing(t_stock *data)
{
	if(data->ac == 5)
	{
		data->file1 = ft_check_file(data->av[1], 1);
		data->file2 = ft_check_file(data->av[4], 0);
		if((data->file1 != 1) && (data->file2 != 1))
		{
			data->cut1 = ft_split(data->av[2], ' ');
			data->cut2 = ft_split(data->av[3], ' ');
			data->cmd1 = ft_command_exist(data->cut1[0], data->env);
			data->cmd2 = ft_command_exist(data->cut2[0], data->env);
			//printf("%s \n", data->cmd1);
			if(data->cmd1 != data->av[2] && data->cmd2 != data->av[3])
				ft_begin_processes(data);
		}
	}
	ft_error(data->ac, data->av, data->env);
}

int main(int ac, char **av, char **env)
{
	t_stock data;
	init(&data, ac, av, env);
	ft_parsing(&data);
	return(0);
}
