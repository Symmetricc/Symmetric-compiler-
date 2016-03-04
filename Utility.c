#include"Header.h"

char *stringCopy(char *Des, char *Sou)
{
	int i = 0;
	while (Sou[i] != '\0')
		i++;
	Des = (char *)malloc(sizeof(char) * i);
	i = 0;
	while ((Des[i] = Sou[i]) != '\0')
		i++;
	return Des;
}
