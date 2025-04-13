

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*ptr;
	size_t	i;

    if (s == NULL)
    {
        return (NULL);
    }
	ptr = (char *)malloc(sizeof(char) * (n + 1));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0' && i < n)
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}