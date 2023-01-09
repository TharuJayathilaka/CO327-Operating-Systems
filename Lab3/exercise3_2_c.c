#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat fixtures | grep <search_term> | cut -b 1-9".
 */

int main(int argc, char **argv)
{
	int status;
	int i;

	// arguments for commands
	char *cat_args[] = {"cat", "fixtures",  NULL};
	char *grep_args[] = {"grep", "-i", argv[1], NULL};
	char *cut_args[] = {"cut", "-b", "1-9", NULL};
	// make 2 pipes (cat to grep and grep to cut); each has 2 fds

	int pipes[4];
	// sets up 1st pipe
	pipe(pipes);

	// sets up 2nd pipe
	pipe(&pipes[2]);

	// we now have 4 fds:
	// pipes[0] = read end of cat->grep pipe (read by grep)
	// pipes[1] = write end of cat->grep pipe (written by cat)
	// pipes[2] = read end of grep->cut pipe (read by cut)
	// pipes[3] = write end of grep->cut pipe (written by grep)

	// fork the first child (to execute cat)

	if (fork() == 0)
	{
		// replace cat's stdout with write part of 1st pipe
		dup2(pipes[1],fileno(stdout));
		
		// close all pipes (very important!); end we're using was safely copied
		close(pipes[0]);
		close(pipes[2]);
        close(pipes[3]);

		// Execute the cat command
		execvp(cat_args[0],cat_args);
		exit(EXIT_SUCCESS);
	}
	else
	{
		// fork second child (to execute grep)

		if (fork() == 0)
		{
			// replace grep's stdin with read end of 1st pipe
			dup2(pipes[0],fileno(stdin));

			// replace grep's stdout with write end of 2nd pipe
			dup2(pipes[3],fileno(stdout));

			// close all ends of pipes
			close(pipes[1]);
			close(pipes[2]);

			// execute grep command
			execvp(grep_args[0],grep_args);
			exit(EXIT_SUCCESS);
		}
		else
		{
			// fork third child (to execute cut)

			if (fork() == 0)
			{
				// replace cut's stdin with input read of 2nd pipe
				dup2(pipes[2],fileno(stdin));

				// close all ends of pipes
                close(pipes[0]);
                close(pipes[1]);
				close(pipes[3]);

				// execute cut command
				execvp(cut_args[0],cut_args);
				exit(EXIT_SUCCESS);
			}
		}
	}

	// only the parent gets here and waits for 3 children to finish
	// It's a good idea to close all your pipes (the parent needs none!)
	// before waiting for your children! 

	close(pipes[0]);
	close(pipes[1]);
    close(pipes[2]);
	close(pipes[3]);
	return 0;

	// TODO: ** insert your code here **
}

