
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0  /* index pipe extremo escritura */
#define WRITE_END 1 /* index pipe extremo lectura */

int	main(int argc, char *argv[])
{
	int	fd1[2], fd2[2], fd3[2];

	int status, pid;
	pipe(fd1); /* comunica ls y grep */
	pid = fork();
	if (pid == 0) /* hijo 1*/
	{
		close(fd1[READ_END]); /* cerrar extremo no necesario */
		dup2(fd1[WRITE_END], STDOUT_FILENO);
		close(fd1[WRITE_END]);
		execlp("/bin/ls", "ls", "-l", NULL);
	}
	else /* padre */
	{
		close(fd1[WRITE_END]); /* extremo no necesario ya */
		pipe(fd2); /* comunica grep y wc */
		pid = fork();
		if (pid == 0) /* hijo 2 */
		{
			close(fd2[READ_END]); /* cerrar extremo no necesario read = [0] */
			dup2(fd1[READ_END], STDIN_FILENO);
			close(fd1[READ_END]);
			dup2(fd2[WRITE_END], STDOUT_FILENO); //write == [1]
			close(fd2[WRITE_END]);
			execlp("/bin/grep", "grep", "u", NULL);
		}
		else /* padre */
		{
			pipe(fd3); /* comunica wc y wc -l */
			close(fd1[READ_END]);  /* cerrar extremo no necesario */
			close(fd2[WRITE_END]); /* cerrar extremo no necesario */
			pid = fork();
			if (pid == 0) /* hijo 3*/
			{
				close(fd3[READ_END]); /* cerrar extremo no necesario */
				dup2(fd2[READ_END], STDIN_FILENO);
				close(fd2[READ_END]);
				//dup2(fd3[WRITE_END], STDOUT_FILENO);
				close(fd3[WRITE_END]);
				execlp("/usr/bin/wc", "wc", "-l", NULL);
			}
		}
	}
	close(fd2[READ_END]); /* cerrar extremo que queda abierto en el padre  */
	/* wait para cada hijo */
	wait(&status);
	wait(&status);
	wait(&status);
	return (0);
}

//#include <readline/history.h>
//#include <readline/readline.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <unistd.h>

// int	main(void)
//{
//	int	fd[2], fd2[2];

//	int status, pid;
//	pipe(fd);
//	if (pid == 0)
//	{
//		close(fd[0]);
//		dup2(fd[1], 1);
//		close(fd[1]);
//		execlp("/bin/ls", "ls", "-l", NULL);
//	}
//	else
//	{
//		close(fd[1]);
//		pipe(fd2);
//		pid = fork();
//		if (pid == 0)
//		{
//			close(fd2[0]);
//			dup2(fd[0], 0);
//			close(fd[0]);
//			dup2(fd2[1], 1);
//			close(fd2[1]);
//			execlp("/bin/grep", "grep", "u", NULL);
//		}
//		else
//		{
//			close(fd[0]);
//			close(fd2[1]);
//			pid = fork();
//			if (pid == 0)
//			{
//				dup2(fd2[0], 0); // stdin
//				close(fd2[0]);
//				execlp("/usr/bin/wc", "wc", "-l", NULL);
//			}
//		}
//	}
//	close(fd2[0]);
//	waitpid(pid, &status, 0);
//	waitpid(pid, &status, 0);
//	waitpid(pid, &status, 0);

//	return (0);
//}
//// int	main(void)
////{
////	char *input;
////	input = readline("Enter a line of text: ");
////	printf("You entered: %s\n", input);
////	rl_replace_line("This is the new line", 0);
////	rl_redisplay();
////	input = readline("Enter another line of text: ");
////	printf("You entered: %s\n", input);
////	return (0);
////}

//// int	main(void)
////{
////	while (1)
////	{
////		char *input = readline("Enter a command: ");
////		printf("You entered: %s\n", input);
////		rl_on_new_line();
////	}
////	return (0);
////}

//// int	main(void)
////{
////	char *input;
////	input = readline("Enter a line of text: ");
////	printf("You entered: %s\n", input);
////	rl_replace_line("This is the new line", 0);
////	rl_redisplay();
////	input = readline("Enter another line of text: ");
////	printf("You entered: %s\n", input);
////	return (0);
////}