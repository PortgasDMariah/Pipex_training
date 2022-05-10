/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouiri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:16:38 by mdouiri           #+#    #+#             */
/*   Updated: 2022/05/07 20:02:05 by mdouiri          ###   ########.fr       */
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
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(av, 2);
	ft_putstr_fd(": command not found\n", 2);	
}


void	ft_error(int ac, char **av, char **env)
{
	char	*cmd1;
	char	*cmd2;

	cmd1 = ft_command_exist(av[2], env);
	cmd2 = ft_command_exist(av[3], env);
	if(ac != 5)
	{
		write(2, "Invalid nb of args \n", 20); 
		exit(1);
	}
	if(ft_check_file(av[1], 1) == 1)
	{
		write(2, "bash: ", 6);
		perror(av[1]);
		if(!cmd2)
		{
			ft_command_not_found(av[3]);
			exit(127);
		}
		exit(0);
	}
	if(ft_check_file(av[4], 0) == 1)
	{
		write(2, "bash: ", 6);
		perror(av[4]);
		if(!cmd1)
			ft_command_not_found(av[2]);
		exit(1);
	}
	if(!cmd1)
		ft_command_not_found(av[2]);
	if(!cmd2)
	{
		ft_command_not_found(av[3]);
		exit(127);
	}
}

//void	ft_begin_processes(char *cmd1, char *cmd2, char **av, char **env)
void	ft_begin_processes(t_stock *data)
{
	int	fd[2];
	pid_t	pid;
	int	status;
//	char	**split;
//	char	**split2;
	
	dup2(data->file1, 0);
	dup2(data->file2, 1);
	status = 0;
	pipe(fd);
	pid = fork();
	//split = ft_split(av[2], ' ');
	//split2 = ft_split(av[3], ' ');
	if(pid == -1)
		return;
	else if(pid == 0)
	{
		/*
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]); 
		execve(cmd1, split, env);
		//close(fd[0]);
		*/
		dup2(fd[0], 0);
	//	dup2(data->file2, 1);
	//	dup2(data->file1, 0);
		close(fd[1]);
		//dup2(fd[0], 0);
		close(fd[0]);
		//execve(data->cmd1, data->cut1, data->env);
		execve(data->cmd2, data->cut2, data->env);
	//	waitpid(pid, &status, 0);

	}
	else
	{
		/*
		//dup2(fda, 0);
		close(fd[0]);
		dup2(fd[1], 1);
		//dup2(fdo, 1); 
		execve(cmd2, split2, env);
		close(fd[1]);
		waitpid(pid, &status, 0);
		//close(fd[1]);
		*/
		//dup2(fd[0], data->file1);
		close(fd[0]);
	//	dup2(data->file1, 0);
		dup2(fd[1], 1);
	//	execve(data->cmd2, data->cut2, data->env);
		execve(data->cmd1, data->cut1, data->env);
		close(fd[1]);
//		waitpid(pid, &status, 0);
//	waitpid(0, NULL, 0);
	}
	close(data->file1);
	close(data->file2);
	//waitpid(0, NULL, 0);
}
#include <stdio.h>

//int	ft_parsing(int ac, char **av, char **env)
void	ft_parsing(t_stock *data)
{
	/*
	char	*cmd1;
	char	*cmd2;
	int	a;
	int	b;
	*/
	if(data->ac == 5)
	{
		data->file1 = ft_check_file(data->av[1], 1);
		data->file2 = ft_check_file(data->av[4], 0);
		if((data->file1 != 1) && (data->file2 != 1))
		{
			//dup2(a, 0);
			//dup2(b, 1);
			/*
			cmd1 = ft_command_exist(av[2], env);
			cmd2 = ft_command_exist(av[3], env);
			*/
			//char	**a1;
			data->cut1 = ft_split(data->av[2], ' ');
		//	char	**a2;
			data->cut2 = ft_split(data->av[3], ' ');
			data->cmd1 = ft_command_exist(data->cut1[0], data->env);
			data->cmd2 = ft_command_exist(data->cut2[0], data->env);
			//printf("%s \n", data->cmd1);
			if(data->cmd1 && data->cmd2)
			{
				//printf("rgwlh \n");
				ft_begin_processes(data);}
				//ft_begin_processes(data->cmd1, data->cmd2, data->av, data->env);
		}
	}
	ft_error(data->ac, data->av, data->env);
}

int main(int ac, char **av, char **env)
{
	/*
	int fd[2];
	pipe(fd);
	int	n;
	int	i;
	int	j = open("me",O_CREAT | O_RDWR | O_TRUNC, 0644);
	i = open("te", O_RDONLY);
	n = fork();
	if(n == 0)
	{
		close(fd[1]);
		//i++;
		//dup2(fd[0], i);
		//printf("i = %d \n", i);
		close(fd[0]);
	}
	else
	{
		close(fd[0]);
		//i++;
		//dup2(fd[1], j);
		//printf("bye et i = %d\n", i);
		close(fd[1]);
	}
	dup2(j ,i);
	*/
//	int a;
//	int b;
//	a = ft_check_file(av[1], 1);
//	b = ft_check_file(av[4], 2);
//	dup2(a, 0);
//	dup2(b , 1);
//	close(a);
//	close(b);
	t_stock data;
	init(&data, ac, av, env);
	ft_parsing(&data);
	free(data.cut1[0]);
	free(data.cut2[0]);
	if(data.cut1[1])
		free(data.cut1[1]);
	if(data.cut2[1])
		free(data.cut2[1]);
	if(data.cmd1)
		free(data.cmd1);
	if(data.cmd2)
		free(data.cmd2);
	
	//free(data->split);
	//free(data->split2);
	//close(a);
	//close(b);
	return(0);
}