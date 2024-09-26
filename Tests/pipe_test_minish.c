#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	main(char **envp)
{
	int		fd[2];
	int		fd2[2];
	int		id;
	int		fd3[2];
	char	cmd[] = "/bin/wc";
	char	*argvec[] = {"wc", "-l", NULL};
	int		fd4;
	int		fd1[2];

	pipe(fd1);
	fd[1] = open("test.txt", O_RDWR);
	id = fork();
	if (id == 0)
	{
		close(fd1[0]);
		fd1[1] = dup2(fd[1], STDOUT_FILENO); ----->> 
		write(fd1[1], "Hella\n", 6);
		//execl("usr/bin/grep", "grep", "-c", "H", fd1, NULL);
		execl("usr/bin/wc", "wc", "-l", fd1, NULL);
		// close(fd1[1]);
	}
	pipe(fd1);
	return (0);
}

// grep "a" | wc -l
// int	fd[2];

// pipe(fd);
// child -> write
// int id = fork();
// char	cmd[] = "/usr/bin/ls";
// char	*argvec[] = {"ls", "-l", NULL};
// char	*envvec[] = {NULL};
// if (id == 0)
//{
//	close(fd[0]);
//	printf("=============================\n");
//	execve(cmd, argvec, envvec);
//	write(fd[1], , sizeof(int));
//}execl("usr/bin/grep", "grep", "-c", stringa, file, NULL)