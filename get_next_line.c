/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:35:07 by ankim             #+#    #+#             */
/*   Updated: 2025/02/11 16:50:34 by ankim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*handle_read(char *buffer, char *temp, int bytes_read)
{
	char	*new_buffer;

	if (bytes_read < 0)
	{
		if (temp)
			free(temp);
		if (buffer)
			free(buffer);
		return (NULL);
	}
	temp[bytes_read] = '\0';
	new_buffer = ft_strjoin (buffer, temp);
	free(buffer);
	if (!new_buffer)
	{
		free(temp);
		return (NULL);
	}
	return (new_buffer);
}

char	*ft_read_into_buffer(int fd, char *buffer)
{
	int		bytes_read;
	char	*temp;

	if (!buffer)
	{
		buffer = ft_calloc(1, 1);
		if (!buffer)
			return (NULL);
	}
	temp = malloc (sizeof(char) * BUFFER_SIZE + 1);
	if (!temp)
	{
		free(buffer);
		return (NULL);
	}
	bytes_read = 1;
	while (bytes_read > 0 && !(ft_strchr(buffer, '\n')))
	{
		bytes_read = read (fd, temp, BUFFER_SIZE);
		buffer = handle_read(buffer, temp, bytes_read);
		if (!buffer)
			return (NULL);
	}
	free(temp);
	return (buffer);
}

char	*ft_make_string(char *buffer)
{
	char	*string;
	int		i;

	if (!buffer || !(*buffer))
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	string = (char *)malloc(sizeof(char) * i + 1);
	if (!string)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		string[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		string[i++] = '\n';
	string[i] = '\0';
	return (string);
}

char	*ft_empty_buffer_used(char **buffer)
{
	int		i;
	int		k;
	char	*remainder;

	i = 0;
	if (!(*buffer)[i])
		return (free(*buffer), NULL);
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	if (!(*buffer)[i] || !(*buffer)[i + 1])
		return (free(*buffer), NULL);
	remainder = (char *)malloc((ft_strlen(*buffer) - i) * sizeof(char));
	if (!remainder)
		return (free(*buffer), NULL);
	k = 0;
	i++;
	while ((*buffer)[i])
	{
		remainder[k++] = (*buffer)[i++];
	}
	remainder[k] = '\0';
	return (free(*buffer), remainder);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*newline;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = ft_read_into_buffer(fd, buffer);
	if (!buffer)
	{
		free(buffer);
		return (NULL);
	}
	newline = ft_make_string(buffer);
	if (!newline)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	buffer = ft_empty_buffer_used(&buffer);
	return (newline);
}

// when your buffer is equals to 10
// 4.KO Timeout but is for the 4.NULL_CHECK.OK 
//so the error is for sure a infini loop !

/*int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	line = get_next_line(fd);
	printf("First call : %s \n", line);
	free(line);
	line = get_next_line(fd);
	printf("second call : %s \n", line);
	free(line);
	close(fd);
	return (0);
}*/
