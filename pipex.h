/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouiri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:00:41 by mdouiri           #+#    #+#             */
/*   Updated: 2022/05/06 18:23:59 by mdouiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

typedef struct s_data
{
	int	ac;
	int	file1;
	int	file2;
	char	**cut1;
	char	**cut2;
	char	*cmd1;
	char	*cmd2;
	char	**env;
	char	**av;
} t_stock;


char	**ft_split(char const *s, char c);
int		ft_check_file(char *str, int tmp);
char	*ft_strnstr(const char *big, const char *little);
char	*ft_strdup(char *s, int i);
int		ft_found_char(char *str, char c);
int		ft_strleni(char *str);
char	*ft_strjoin(char *s1, char *s2);
int		ft_str(char *env, char *find);
char	*ft_path(char **env);
char	*ft_command_exist(char *cmd, char **env);
void	ft_error(int ac, char **av, char **env, t_stock *data);
//void	ft_begin_processes(char *cmd1, char *cmd2, char **av, char **env);
//int		ft_parsing(int ac, char **av, char **env);
void	ft_parsing(t_stock *data);
void	ft_begin_processes(t_stock *data);

#endif
