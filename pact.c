/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pact.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouiri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:16:38 by mdouiri           #+#    #+#             */
/*   Updated: 2022/05/10 20:00:17 by mdouiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <errno.h>

void	init(t_stock *data, int ac, char **av, char **env)
{
	data->ac = ac;
	data->av = av;
	data->env = env;
	data->file1 = 0;
	data->file2 = 0;
	data->fd[0] = 0;
	data->fd[1] = 0;
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
	if(cmd && access(cmd, X_OK) == 0)
		return(cmd);
	while(cmd && path && (ft_found_char(path, ':') != -1))
	{
		path_bis = ft_strdup(path, ft_found_char(path, ':'));
		new_path = ft_strjoin(path_bis, cmd);
		free(path_bis);
		if(cmd && access(new_path, X_OK) == 0)
		{
			return(new_path);
		}
		free(new_path);
		path = path + (ft_found_char(path, ':') + 1);
	}
	return(NULL);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	ft_error(int ac, char **av, char **env, t_stock *data)
{
	(void)env;
	if(ac != 5)
	{
		write(2, "Invalid nb of args \n", 20); 
		exit(1);
	}
	if(data->file1 == -1)
	{
		write(2, "bash: ", 6);
		perror(av[1]);
	}
	if(data->file2 == -1)
	{
		write(2, "bash: ", 6);
		perror(av[4]);
	}
}

void	ft_command_not_found(char *av)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(av, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void	free_well(char	**str, int tmp, char *s)
{
	int	i;

	i = 0;
	if(tmp == 1)
	{
		if(!str)
			return;
		while(str && str[i++])
			free(str[i]);
	}
	else
	{
		if(s)
			free(str);
	}
}

void	child_one(t_stock *data)
{
	close(data->fd[1]);
	dup2(data->fd[0], 0);
	close(data->fd[0]);
	if(data->av[3] && data->av[3][0] != '\0')
	{
		data->cut2 = ft_split(data->av[3], ' ');
		data->cmd2 = ft_command_exist(data->cut2[0], data->env);
	}
	if(!data->cmd2)
	{
		ft_command_not_found(data->av[3]);
		free_well(data->cut2, 1, "");
		exit(127);
	}
	execve(data->cmd2, data->cut2, data->env);
	exit(0);
}

void	child_two(t_stock *data)
{
	close(data->fd[0]);
	dup2(data->fd[1], 1);
	close(data->fd[1]);
	if(data->av[2] && data->av[2][0] != '\0')
	{
		data->cut1 = ft_split(data->av[2], ' ');
		data->cmd1 = ft_command_exist(data->cut1[0], data->env);
	}
	if(!data->cmd1)
	{
		ft_command_not_found(data->av[3]);
		free_well(data->cut1, 1, "");
		exit(1);
	}
	execve(data->cmd1, data->cut1, data->env);
	exit(0);
}


void	ft_begin_processes(t_stock *data)
{
	pid_t	pid;
	pid_t	pid2;
	int	status;

	dup2(data->file1, 0);
	dup2(data->file2, 1);
	status = 0;
	if(pipe(data->fd) == -1)
		return;
	pid = fork();
	if(pid == -1)
		return;
	else if(pid == 0)
	{
		child_one(data);
	}
	else
	{
		pid2 = fork();
		if(pid2 == -1)
			return;
		else if(pid2 == 0)
		{
			child_two(data);
		}
		else
		{
			close(data->fd[0]);
			close(data->fd[1]);
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);
		}
	}
	//close(data->file1);
	//close(data->file2);
}
#include <stdio.h>

void	ft_parsing(t_stock *data)
{
	if(data->ac == 5)
	{
		data->file1 = ft_check_file(data->av[1], 1);
		data->file2 = ft_check_file(data->av[4], 0);
		if((data->file1 != -1) && (data->file2 != -1))
		{
			ft_begin_processes(data);
		}
	}
	ft_error(data->ac, data->av, data->env, data);
}

int main(int ac, char **av, char **env)
{
	if(env)
	{
	t_stock data;
	init(&data, ac, av, env);
	ft_parsing(&data);
	close(data.file1);
	close(data.file2);


	}
	return(0);
}

