#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>

#define	oops(m,x){ perror(m); exit(x); }


void main(int ac, char **av)
{

	int	thepipe1[2];		/* two file descriptors	 Parent-Child */
	int	thepipe2[2];		/* two file descriptors	Child GrandChild */
	int newpid;
      
	if ( ac != 4 )
	{
		fprintf(stderr, "usage: pipe cmd1 cmd2 cmd3\n");
		exit(1);
	}

	if ( pipe( thepipe1 ) == -1 )		/* get a pipe */
	{
		oops("Cannot get a pipe", 1);
	}

	if ( pipe( thepipe2 ) == -1 )
	{		
		oops("Cannot get a pipe", 1);
	}
  /* ------------------------------------------------------------ */
  /*	now we have a pipe, now let's get two processes		*/
	newpid = fork();  //parent

	if ( newpid == -1 )
	{
		perror(" fork error ");
	}
	else if ( newpid == 0 )
	{
	   	int Child1 = fork();

	       	if ( Child1 == -1 )
			{
	        	perror(" fork error ");
	        	
			}
		else if ( Child1 == 0 )
		{
/* child execs av[1] and writes into pipe */

            close(thepipe2[0]); /* Grand_child doesn't read from pipe */

            if ( dup2(thepipe2[1], 1) == -1 )
                oops("could not redirect stdout", 4);

            close(thepipe2[1]);	/* stdout is duped, close pipe	*/
            execlp( av[1], av[1], NULL);
            oops(av[1], 5);
		}
		else
		{
            //Child_Parent read from the GrandChild
			if ( dup2(thepipe2[0], 0) == -1 )
			{
				oops("could not redirect stdin",3);
			}
			close(thepipe2[0]);	/* stdin is duped, close pipe	*/
			/*child execs av[1] and writes into pipe*/

			if ( dup2(thepipe1[1], 1) == -1 )
			{
				oops("could not redirect stdout", 4);
			}
			close(thepipe1[1]);	/* stdout is duped, close pipe	*/
			execlp( av[2], av[2], NULL);
			oops(av[1], 5);

        }

	}
	else
	{    
     	close(thepipe1[1]);	/* parent doesn't write to pipe	*/

		if ( dup2(thepipe1[0], 0) == -1 )
		oops("could not redirect stdin",3);

     	close(thepipe1[0]);
     	execlp( av[3], av[3], NULL);
     	oops(av[3], 4);

	}
}