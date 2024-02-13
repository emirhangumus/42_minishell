#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libft/libft.h"

// int f(int argc, char *argv[])
// {
// 	static char *newargv[] = {"ls",  "a./../libft", NULL };
// 	// static char *newenviron[] = { "/usr/local/bin", "/usr/bin", "/bin", "/usr/sbin", "/sbin", "/usr/local/munki" };

// 	// if (argc != 2) {
// 	// 	printf("%d\n", argc);
// 	// 	fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
// 	// 	exit(EXIT_FAILURE);
// 	// }

// 	// newargv[0] = argv[1];

// 	if (execve("/bin/ls", newargv, NULL))
// 		printf("%s hata", argv[0]);
// 	// perror("execve");   /* execve() returns only on error */
// 	// exit(EXIT_FAILURE);
// }

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tab;
	size_t	i;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = (size_t)strlen(s1);
	len2 = (size_t)strlen(s2);
	tab = (char *)malloc(sizeof(char) * (len1 + len2) + 1);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		tab[i] = s1[i];
		i++;
	}
	while (i < (len1 + len2))
	{
		tab[i] = s2[i - len1];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

char **get_all_env()
{
	extern char **environ;
	char **newenv = (char **)malloc(sizeof(char *) * 1000);
	int i = 0;
	while (environ[i])
	{
		newenv[i] = ft_strdup(environ[i]);
		i++;
	}
	newenv[i] = NULL;
	return (newenv);
}

int tab_len(char **tab)
{
	int i = 0;
	while (tab[i])
		i++;
	return (i);
}

int main(int ac, char **av)
{
	// static char *newarg[64] = {};
	char *s;

	char **j = get_all_env();

	while (1)
	{
		int pid = fork();
		if (pid == 0)
		{
			s = readline("Sexy Minishell -> ");
			add_history(s);
			if ('E' == s[0])
				break;
			s = ft_strjoin("/bin/", s);
			char **gay = ft_split(s, ' ');
			printf("%s\n", s);
			if (execve(gay[0], gay, j))
				perror("execve");
			// kill(getpid(), SIGKILL);
		}
		else
			waitpid(pid, NULL, 0);
	}
}

// int main()
// {
// 	pid_t pid;

// 	pid = fork();

// 	printf("pid->%d\n", getpid());
// 	printf("pid2->%d\n", pid);
// }