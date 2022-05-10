/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouiri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:16:38 by mdouiri           #+#    #+#             */
/*   Updated: 2022/05/10 19:53:10 by mdouiri          ###   ########.fr       */
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

void	ft_command_not_found(char *av)
{
	//free(cmd);
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(av, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void    ft_free_split(char **str)
{
        int i;
        i = 0;
	if(!str)
		return;
        while(str[i++])
                free(str[i]);
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

void	free_all(t_stock *data)
{
	free_well(data->cut1, 1, "");
	free_well(data->cut2, 1, "");
	free_well(NULL, 2, data->cmd1);
	free_well(NULL, 2, data->cmd2);
}

void	ft_check_error(t_stock *data)
{
	if(data->ac != 5)
	{
		write(2, "Invalid nb of args \n", 20);
		exit(0);
	}
	if(data->file2 == -1)
	{
		write(2, "bash: ", 6);
                perror(data->av[4]);
		free_all(data);
		exit(0);
	}
	if(data->file1 == -1)
	{
		write(2, "bash: ", 6);
                perror(data->av[1]);
		free_all(data);
		exit(0);
	}
	if(!data->cmd1)
	{
		ft_command_not_found(data->av[2]);
	}
	if(!data->cmd2)
	{
		ft_command_not_found(data->av[3]);
	}
	free_all(data);
	exit(0);
}


void	ft_begin_processes(t_stock *data)
{
	int	fd[2];
	pid_t	pid;
	pid_t	pid2;
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
		dup2(fd[0], 0); //ca on remet au cas ou
		close(fd[1]);
		close(fd[0]); // on remet
		if(data->cmd2)
			{if(execve(data->cmd2, data->cut2, data->env) == -1)
				perror("exce");
			}
		//ft_free(data, 1);
		//ft_free(data, 2);
		free_all(data);
		exit(0);
	}
	else
	{
		pid2 = fork();
		if(pid2 == -1)
			return;
		else if(pid2 == 0)
		{
			close(fd[0]);
			dup2(fd[1], 1);//on remet ca
			if(data->cmd1)
			{
				if (execve(data->cmd1, data->cut1, data->env) == -1)
					perror("execve");
			}
			close(fd[1]);
		//	ft_free(data, 1);
		//	ft_free(data, 2);
			free_all(data);
			exit(0);
		}
		else
		{
			
			close(fd[0]);
			close(fd[1]);
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);
		}
		
	}
	close(data->file1);
	close(data->file2);
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
			if(data->av[2] && data->av[2][0] != '\0')
			{
				data->cut1 = ft_split(data->av[2], ' ');
				data->cmd1 = ft_command_exist(data->cut1[0], data->env);
			}
			if(data->av[3] && data->av[3][0] != '\0')
			{
				data->cut2 = ft_split(data->av[3], ' ');
				data->cmd2 = ft_command_exist(data->cut2[0], data->env);
			}
			if(data->cmd1 || data->cmd2)
				ft_begin_processes(data);
		}
	}
	ft_check_error(data);
}

int main(int ac, char **av, char **env)
{
	if(env)
	{
	t_stock data;
	init(&data, ac, av, env);
	ft_parsing(&data);
	}
	return(0);
}

