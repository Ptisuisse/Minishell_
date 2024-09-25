#include <stdio.h>
#include <string.h>
#include <unistd.h>

int	main(int argc, char *argv)
{
	int	id;
	int	id2;

	id = fork();
	if (id == 0)
	{
		printf("id1 =%d\n", id);
		id2 = fork();
		if (id2 == 0)
		{
			printf("id2 = %d\n", id2);
			printf("id1 acte 2%d\n", id);
		}
	}
	return (0);
}

// lancer 2 fork
// 1er fork = main + child
// 2eme fork = main + child
// child + childofchild