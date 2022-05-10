/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p1005.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouiri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:16:38 by mdouiri           #+#    #+#             */
/*   Updated: 2022/05/10 18:46:56 by mdouiri          ###   ########.fr       */
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

	//if(!cmd || cmd[0] == '\0')
	//	return(NULL);
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

void	ft_command_not_found(char *av, char *cmd)
{
	free(cmd);
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
//	free(str[i]);

}

void	ft_free(t_stock *data, int tmp)
{
	if(tmp == 2)
	{
		/*
		if(data->cut2)
			free(data->cut2[0]);
		if(data->cut2 && data->cut2[1])
			free(data->cut2[1]);
		*/
		ft_free_split(data->cut2);
		if(data->cmd2)
			free(data->cmd2);
	//	if(data->cut2)
	//		free(data->cut2);
	}
	else if(tmp == 1)
	{
		/*
		if(data->cut1)
			free(data->cut1[0]);
		if(data->cut1 && data->cut1[1])
			free(data->cut1[1]);
		*/
		ft_free_split(data->cut1);
		if(data->cmd1)
			free(data->cmd1);
	//	if(data->cut1)
	//		free(data->cut1);
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
		if(data->cmd2)
			ft_free(data, 2);
		if(data->cmd1)
			ft_free(data, 1);
		//if(data->file2 != -1)
			close(data->file2);
		exit(0);
	}
	if(data->file2 == -1)
	{
		write(2, "bash: ", 6);
		perror(av[4]);
		if(data->cmd1)
			ft_free(data, 1);
		if(data->cmd2)
			ft_free(data, 2);
		close(data->file1);
		exit(1);
	}
	if(data->file1 && !data->cmd1)
	{
		ft_command_not_found(av[2], data->cmd1);
		close(data->file1);
	}
	if(data->file2 && !data->cmd2)
	{
	//	printf("mamame \n");
		ft_command_not_found(av[3], data->cmd2);
		//free(cmd2);
		/*
		if(data->file1)
			close(data->file1);
		*/
		if(data->cmd1)
			ft_free(data, 1);
		ft_free(data, 2);
		close(data->file2);
		close(data->file1);
		exit(127);
	}

	if(!data->cmd1 && data->cmd2)
		free(data->cmd2);
	close(data->file2);
	//exit(0);
	/*
	if(data->file1)
		close(data->file1);
	if(data->file2)
		close(data->file2);
	*/
	/*
	if(data->cmd1 && data->cmd2)
	{
		free(data->cmd1);
		free(data->cmd2);
	}
	*/
}

void	ft_begin_processes(t_stock *data)
{
	int	fd[2];
	//int fd2[2];
	pid_t	pid;
	pid_t	pid2;
	int	status;
	//pid_t	wpid;
	dup2(data->file1, 0);
	dup2(data->file2, 1);
	status = 0;
	pipe(fd);
	//pipe(fd2);
	//int	a = 0;
	//ft_error(data->ac, data->av, data->env, data);
	pid = fork();
	if(pid == -1)
		return;
	else if(pid == 0)
	{
		dup2(fd[0], 0); //ca on remet au cas ou
		//dup2(data->file1, fd[0]);
		close(fd[1]);
		close(fd[0]); // on remet
		//if (!data->cmd2 || dat)
		//	ft_error(data->ac, data->av, data->env, data);
		if(data->cmd2)
			{if(execve(data->cmd2, data->cut2, data->env) == -1)
				perror("exce");
			}
		ft_free(data, 1);
		ft_free(data, 2);
		exit(0);
		/*if(a == -1)
			return;*/
	//	waitpid(pid, &status, 0);

	}
	else
	{
		pid2 = fork();
		if(pid2 == -1)
			return;
		else if(pid2 == 0)
		{
			/*
			dup2(fd2[0], 0);
			close(fd2[1]);
			close(fd2[0]);
			*/
			//a = 0;
			close(fd[0]);
			
		//	if (!data->cmd1 || data->file1 )
				//ft_error(data->ac, data->av, data->env, data);
			dup2(fd[1], 1);//on remet ca
			//dup2(data->file1, data->file2); 
			//dup2(data->file2, fd[1]);
			if(data->cmd1)
			{
				if (execve(data->cmd1, data->cut1, data->env) == -1)
					perror("execve");
			}
			//if(a == -1)
			//	return;
			close(fd[1]);
			ft_free(data, 1);
			ft_free(data, 2);
			exit(0);
			/*
			execve(data->cmd1, data->cut1, data->env);
			*/
		}
		else
		{
			
			//while((wpid = wait(&status)) > 0);
				//	;
			//while(wait(NULL) != -1 || errno != ECHILD)
			//	;
			close(fd[0]);
			close(fd[1]);
		//	close(fd2[0]);
		//	close(fd2[1]);
		/*
			close(1);
			close(0);
			close(2);
		*/	
			
			//dup2(fd[1], 1);
			//dup2(fd2[1], 1);
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);
		}
		
	}
	//ft_free(data, 1);
        //ft_free(data, 2);
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
	//ft_free(data, 1);
          //              ft_free(data, 2);
	ft_error(data->ac, data->av, data->env, data);
}

int main(int ac, char **av, char **env)
{
	if(env)
	{
	t_stock data;
	init(&data, ac, av, env);
	//printf("grberb \n");
	ft_parsing(&data);
	//printf("eslgihz \n");
	//printf("gerh \n");
	if(av[2])
	{
	//	printf("derib \n");
		if(data.cut1 && data.cut1[0])
			free(data.cut1[0]);
		if(data.cut1 && data.cut1[1])
			free(data.cut1[1]);
		if(data.cmd1)
			free(data.cmd1);
		if(data.cut1)
			free(data.cut1);
	}
	if(av[3])
	{
	//	printf("srpqjb \n");
		if(data.cut2 && data.cut2[0])
			free(data.cut2[0]);
		if(data.cut2 && data.cut2[1])
			free(data.cut2[1]);
		if(data.cmd2)
			free(data.cmd2);
		if(data.cut2)
			free(data.cut2);
	}
	//close(a);
	//close(b);
	}
	return(0);
}

