/********************************************************
 *
 * shell.c - Shell Application
 *
 * Author : Tzvi Moskowitz
 *
 * Reviewed and approved by : Tuvia
 *
 * November, 9 -2021
 *
 * OLRD 110 Ramat Gan
 *
 * *********************************************************/

#include <stdio.h>  /* printf   */
#include <stdlib.h> /* system   */
#include <string.h> /* strncmp  */
#include <unistd.h> /*fork*/
#include <signal.h> /* signals  */
#include <sys/wait.h>  /* wait  */
#include <sys/types.h> /* pid_t */

#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"
#define _YELLOW_ "\033[1;33m"
#define _BLUE_   "\x1B[1;34m"
#define _PURPLE_ "\033[1;35m"
#define _CYAN_   "\033[1;36m"
#define _WHITE_  "\033[1;0m"
#define _RESET_   "\033[1;0m"

#define BUF_SIZE 100
#define EXIT_LEN 4


static int StartShellForkImple(char *program_name);
static int StartShellSystemImple(void);
static void GetUserCommand(char *command_buffer, size_t size);

static void ParseCommand(char *arg_list[], char *command_buffer);
static void HandleForExitCall(char *buf);
static void PromptImpleChoiceError(void);

static char *Mystrtok_r(char *s, const char *delim, char **save_ptr);


int main(int argc, char **argv)
{
	(void)argc;

	if (argc <= 1)
	{
		PromptImpleChoiceError();
		return 0;
	}
	else if (0 == strcmp("fork", argv[1]))
	{
		return StartShellForkImple(argv[0]);
	}
	else if (0 == strcmp("system", argv[1]))
	{
		return StartShellSystemImple();
	}
	else
	{
        PromptImpleChoiceError();

		return 0;
	}
}


static int StartShellForkImple(char *program_name)
{
    char command_buffer[BUF_SIZE] = {""};

	while (1)
	{

		pid_t pid = 0;
		GetUserCommand(command_buffer, BUF_SIZE);



		command_buffer[strlen(command_buffer) - 1] = 0;
		printf("command: -%s-\n", command_buffer);
		HandleForExitCall(command_buffer);

		pid = fork();
        if (-1 == pid)
		{
			printf(_RED_"fatal error: could not fork \n"_RESET_"");
			exit(1);
		}
		else if (0 == pid)
        {
            char *arg_list[10] = {NULL};





    		ParseCommand(arg_list, command_buffer);



            if (-1 == execvp(arg_list[0], arg_list))
			{
				printf ("sorry did not work;");
				exit(1);
			}

        }
		else
		{
			int status = 0;

			wait( &status);
			if (0 != status)
			{
				printf(_YELLOW_"\n command ecexution failed\n"_RESET_"");

			}
		}

	}
    return 0;
}



static int StartShellSystemImple()
{
    char command_buffer[BUF_SIZE] = {""};
    int status = 0;

	while (1)
	{
        GetUserCommand(command_buffer, BUF_SIZE);

        HandleForExitCall(command_buffer);

        status = system(command_buffer);

        if (status != 0)
		{
			printf("cound not carry out command.\n");
        }

	}

	return 0;
}


static void ParseCommand(char *arg_list[], char *command_buffer)
{
	size_t i = 0;
    char *next_token = command_buffer;

	arg_list[0] = strtok(command_buffer, " ");
	while (arg_list[i] != NULL)
	{
		arg_list[++i] = strtok(NULL, " ");

	}
}

static char *Mystrtok_r(char *s, const char *delim, char **save_ptr)
{
    char *end;
  	if (s == NULL)
    s = *save_ptr;
  	if (*s == '\0')
  	{
	  *save_ptr = s;
	  return NULL;
	}
  	/* Scan leading delimiters.  */
  	s += strspn (s, delim);
  	if (*s == '\0')
    {
      *save_ptr = s;
      return NULL;
    }
  	/* Find the end of the token.  */
  	end = s + strcspn (s, delim);
  	if (*end == '\0')
    {
      *save_ptr = end;
      return s;
    }
  	/* Terminate the token and make *SAVE_PTR point past it.  */
  	*end = '\0';
  	*save_ptr = end + 1;
  	return s;
}

static void HandleForExitCall(char *buf)
{
	if (0 == strncmp(buf, "exit", EXIT_LEN))
	{
		exit(0);
	}
}


static void PromptImpleChoiceError()
{
	printf(_RED_"error: you need to choose option " _CYAN_"\"fork\""_RED_" \
    or "_CYAN_"\"system\".\n"_RESET_"");
}

static void GetUserCommand(char *command_buffer, size_t size)
{
    printf("\n"_GREEN_"enter a command"_CYAN_" --> "_RESET_);

    if(!fgets(command_buffer, size, stdin))
    {
        printf(_RED_"\nCould not read from stdin\n"_RESET_"");
        exit(1);
    }

}
static void PrintError()
{
	printf("error. terminating.\n");
}
