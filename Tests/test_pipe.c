#include <stdio.h>
#include <unistd.h>

int	main(int argc, char *argv)
{
	int fd[2];
	// fd[0] - read
	// fd[1] - write
	if (pipe(fd) == -1) // fd pour le pipe &
	{
		printf("ERROR OPEN FD\n");
		return (1);
	}
	int id = fork();
	//Error ID == -1
	if (id == 0)
	{
		close(fd[0]);
		int x;
		printf("Input a number : ");
		scanf("%d", &x);
		if (write(fd[1], &x, sizeof(int)) == -1)
			return (2);
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		int y;
		if (read(fd[0], &y, sizeof(int)) == -1)
			return (3);
		close(fd[0]);
		printf("Got from child process %d\n", y);
	}
	return (0);
}