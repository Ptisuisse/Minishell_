#include "minishell.h"

int	check_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		// Le chemin n'existe pas
		return (0);
	}
	// Vérifie si c'est un dossier
	return (S_ISDIR(path_stat.st_mode));
}

int	check_file(const char *filename, t_command *commands)
{
	struct stat filestat;

	// Vérifie si le chemin est un dossier
	if (stat(filename, &filestat) == 0 && S_ISDIR(filestat.st_mode))
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" is a directory");
		return (2);
	}

	// Vérifie si le dossier contenant le fichier existe
	char *dirname = strdup(filename);
	char *last_slash = strrchr(dirname, '/');
	if (last_slash != NULL)
	{
		*last_slash = '\0';
		if (!check_directory(dirname))
		{
			commands->error_file = 1;
			commands->error_message = ft_strdup(" No such file or directory");
			free(dirname);
			return (1);
		}
	}
	free(dirname);

	// Vérifie si le fichier existe
	if (access(filename, F_OK) == -1)
	{
		// Le fichier n'existe pas, il sera créé plus tard
		return (0);
	}

	// Vérifie les permissions d'écriture
	if (access(filename, W_OK) == -1)
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" Permission denied");
		return (1);
	}

	return (0);
}