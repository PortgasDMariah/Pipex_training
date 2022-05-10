/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouiri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:00:28 by mdouiri           #+#    #+#             */
/*   Updated: 2022/05/10 17:19:59 by mdouiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

static int      ft_charset(char charset, char a)
{
        if (charset == a)
                return (1);
        else
                return (0);
}

static int      ft_word(char sep, char *str)
{
        int     i;
        int     wd;

        i = 0;
        wd = 0;
        while (ft_charset(sep, str[i]) == 1 && str[i])
                i++;
        while (str[i])
        {
                wd++;
                while (ft_charset(sep, str[i]) == 0 && str[i])
                        i++;
                while (ft_charset(sep, str[i]) == 1 && str[i])
                        i++;
        }
        return (wd);
}

static char     *ft_strdupi(const char *str, char sep)
{
        int             i;
        int             v;
        char    *res;

        i = 0;
        v = 0;
        while (str[i] && ft_charset(sep, str[i]) == 0)
                i++;
        res = (char *)malloc((i + 1) * sizeof (char));
        i = 0;
        if (!res)
                return (0);
        while (ft_charset(sep, str[i]) == 1 && str[i])
                i++;
        while (ft_charset(sep, str[i]) == 0 && str[i])
        {
                res[v] = str[i];
                i++;
                v++;
        }
        res[v] = '\0';
        return (res);
}
/*
void	ft_free_split(char **str)
{
	int i;
	i = 0;
	while(str[i++])
		free(str[i]);

}
*/

char    **ft_split(char const *s, char c)
{
        int             i;
        int             v;
        char    **split;

        if (!s)
                return (NULL);
        split = (char **)malloc((ft_word(c, ((char *)s)) + 1) * sizeof(char *));
        if (!split)
                return (0);
        v = 0;
        i = 0;
        while (ft_charset(c, s[i]) == 1 && s[i])
                i++;
        while (s[i])
        {
                split[v] = ft_strdupi(&s[i], c);
                if (!split[v])
		{
			//ft_free_split(split);
                        return (0);
		}
                while (ft_charset(c, s[i]) == 0 && s[i])
                        i++;
                while (ft_charset(c, s[i]) == 1 && s[i])
                        i++;
                v++;
        }
        split[ft_word(c, ((char *)s))] = 0;
        return (split);
}

int ft_strleni(char *str)
{
        int i;
        i = 0;
        if(!str)
                return(0);
        while(str[i])
                i++;
        return(i);
}

char    *ft_strjoin(char *s1, char *s2)
{
        char    *res;
        int             i;
        int             v;

        res = malloc((ft_strleni(s1) + ft_strleni(s2) + 2) * sizeof(char));
        if (!res)
                return (0);
        i = 0;
        v = 0;
        while (s1[i])
        {
                res[v] = s1[i];
                i++;
                v++;
        }
        res[v] = '/';
        v++;
        i = 0;
        while (s2[i])
        {
                res[v] = s2[i];
                i++;
                v++;
        }
        res[v] = '\0';
        return (res);
}

int     ft_str(char *env, char *find)
{
        int i;

        i = 0;
        if(!env)
                return(0);
        while(env[i] == find[i])
                i++;
        if(find[i] == '\0')
                return(1);
        return(0);
}

char    *ft_path(char **env)
{
        char    **enva;
        int     i;

        i = 0;
        enva = env;
        while (enva[i] && (ft_str(enva[i], "PATH=") == 0))
                i++;
        if (!enva[i])
                return (NULL);
        return (enva[i]);
}


char	*ft_strdup(char *s, int i)
{
	int		j;
	char	*res;

	res = malloc((i + 1) * sizeof (char));
	if (!res)
		return (0);
	j = 0;
	while (s[j] && j < i)
	{
		res[j] = s[j];
		j++;
	}
	res[j] = '\0';
	return (res);
}

int	ft_found_char(char *str, char c)
{
	int	i;

	i = 0;
	if(!str)
		return(-1);
	while(str[i] && str[i] != c)
		i++;
	if(str[i] == c)
		return(i);
	return(-1);

}

int	ft_check_file(char *str, int tmp)
{
	int	fd;

	if(tmp == 1)
		fd = open(str, O_RDONLY);
	else
		fd = open(str, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if(fd < 0)
		return(-1);
	return(fd);
}
