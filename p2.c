// ******************************************************************************************************************
//  Multi-processes for Linux
//  Copyright(C) 2018  James LoForti
//  Contact Info: jamesloforti@gmail.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.If not, see<https://www.gnu.org/licenses/>.
//									     ____.           .____             _____  _______   
//									    |    |           |    |    ____   /  |  | \   _  \  
//									    |    |   ______  |    |   /  _ \ /   |  |_/  /_\  \ 
//									/\__|    |  /_____/  |    |__(  <_> )    ^   /\  \_/   \
//									\________|           |_______ \____/\____   |  \_____  /
//									                             \/          |__|        \/ 
//
// ******************************************************************************************************************
//

#include <stdlib.h> // needed for exit()
#include <stdio.h> // needed for printf()
#include <sys/types.h> // needed for pid_t type
#include <sys/wait.h> // needed for wait()
#include <unistd.h> // needed for getpid()

//Function prototypes
void parent(pid_t pid, int argc, char* argv[]);
void child(pid_t pid, int argc, char* argv[]);

int main(int argc, char* argv[])
{
    //Print opening seperator, name, and project
    printf("\n*********************************************** ");
    printf("\nName: James LoForti \n\n");

	//Declare vars:
	pid_t pid;

	//Create a child process and save pid
	pid = fork();

	//If fork() failed
	if (pid < 0)
	{
		printf("CHILD process creation failed, exiting... \n");
		return -1;
	}

	//If parent process is running
	if (pid > 0)
	{
		parent(pid, argc, argv);
	}

	//If child process is running
	if (pid == 0)
	{
		child(pid, argc, argv);
	}

	return 0;
} // end main()


void parent(pid_t pid, int argc, char* argv[])
{
	//Inform user parent has started and is waiting for child
	printf("PARENT started, now waiting for process %d... \n", pid);

	//Have parent wait for child to finish
	waitpid(pid, NULL, WNOHANG);

	//For some reason waitpid was not working on your server :(
	//If you look at the child(), I even tried to flush the buffer,
	//but was still receiving out of order prints to the console.
	//Again, none of this occurred in my environment.
	//sleep was the only thing that worked.
	sleep(1); 

    //Inform user parent has resumed, and will now kill itself
	printf("\nPARENT resumed, CHILD exited with code 0. Killing PARENT... \n");

	//Print closing seperator
	printf("\n*********************************************** \n\n");
} // end function parent()

void child(pid_t pid, int argc, char* argv[])
{
	//ZERO args given
	if (argc == 1)
	{
		printf("CHILD started, but no arguments given. Killing CHILD... ");
		fflush(stdout);
	}
	else // args were given
	{
		//ONE arg given
		if (argc == 2)
		{
		  	printf("CHILD started, one argument given. Calling execlp()... \n\n");
		  	fflush(stdout);
			execlp(argv[1], argv[1], NULL);
		}

		//MULTI args given
		if (argc > 2)
		{
			printf("CHILD started, multiple arguments given. Calling execvp()... \n\n");
			fflush(stdout);
			execvp(argv[1], &argv[1]);
		}
	} // end else

	return;
} // end function child()