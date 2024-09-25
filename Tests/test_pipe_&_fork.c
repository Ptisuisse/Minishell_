#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

	int main(int argc, char *argv)
{
	int fd = open("test.txt", O_RDWR);
	// fd[0] - read
	// fd[1] - write
	if (pipe(fd) == -1) // fd pour le pipe &
	{
		printf("ERROR OPEN FD\n");
		return (1);
	}
	int id = fork();
	if (id == 0)
	{
		close(fd[]);
		write(fd[], "Test", 4);
		close(fd[]);
	}
	else
	{
		close(fd[]);
		read(fd[0], fd[1], 4);
		printf("%d", fd[1]);
		close(fd[0]);
	}
	return (0);
}