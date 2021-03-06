/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astanton <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 15:24:26 by astanton          #+#    #+#             */
/*   Updated: 2019/01/18 15:28:42 by astanton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"
#include "get_next_line.h"

static t_list	*get_current_list(int fd, t_list **head)
{
	t_list	*current;

	current = *head;
	while (current)
	{
		if ((int)current->content_size == fd)
			return (current);
		current = current->next;
	}
	current = ft_lstnew(NULL, fd);
	ft_lstadd(head, current);
	current = *head;
	(*head)->content_size = fd;
	return (current);
}

static char		*ft_strjoin_free(char *s1, char *s2)
{
	char	*str;

	str = ft_strjoin(s1, s2);
	free(s1);
	return (str);
}

static int		save_line(char **line, char *buf, t_list *current)
{
	int		i;
	char	*str;

	i = -1;
	str = ft_strnew(ft_strlen(buf));
	while (buf[++i] != '\n' && buf[i] != '\0')
		str[i] = buf[i];
	str[i] = '\0';
	*line = ft_strjoin_free(*line, str);
	free(str);
	if (buf[i++] != '\0')
	{
		str = ft_strjoin("", buf + i);
		if (current->content)
			free(current->content);
		current->content = ft_strjoin_free(str, "");
	}
	else
	{
		if (current->content != NULL)
			free(current->content);
		current->content = NULL;
	}
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	char			buf[BUFF_SIZE + 1];
	static t_list	*head;
	t_list			*current;
	int				ret;

	if (fd < 0 || !line || (read(fd, buf, 0)) < 0)
		return (-1);
	current = get_current_list(fd, &head);
	*line = ft_strnew(1);
	if (current->content && ft_strchr(current->content, '\n'))
		return (save_line(line, current->content, current));
	else if (current->content)
		save_line(line, current->content, current);
	while ((ret = read(fd, buf, BUFF_SIZE)))
	{
		buf[ret] = '\0';
		if (ft_strchr(buf, '\n'))
			return (save_line(line, buf, current));
		*line = ft_strjoin_free(*line, buf);
	}
	if (ft_strlen(*line))
		return (1);
	else
		return (0);
}
