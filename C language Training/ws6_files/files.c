#include <stdio.h> /*  printf etc. */
#include <stdlib.h> /*exit*/
#include <string.h>  /*strcmp etc.*/
#include <assert.h> /*assert*/

 /*APROVED BY SHACHAR*/

enum error_handle{SUCCESS,STDIN_FAILURE,FOPEN_FAIL,FILE_WRITE_FAIL,FCLOSE_FAIL,REMOVE_FAILURE,RENAME_FAILURE};
typedef struct flagger
{
	char *word;
	int (*compare)(const char *, const char *);
	int (*operation)(char *,char *);
}flagger;

int CountLines(char *,char *);
int RemoveFile(char *, char *);
int ExitProgram(char *,char *);
int AddToBeginning(char *,char *);
int AddToEnd(char *, char *);
void ErrorHandle( enum error_handle);
int AppendText(char * );
int DefaultCompare(const char *, const char* );
int CompareFirstChar(const char *, const char *);



/*
flagger REMOVE = ;
flagger EXIT = ;
flagger COUNT = ;
flagger ADD_BEGINNING = ;
flagger APPEND = ;


flagger *p1, *p2, *p3, *p4, *p5;
p1 = REMOVE; p2 = EXIT;  p3 = COUNT; p4 = ADD_BEGINNING; p5 = APPEND;
*/

flagger flag_controller[5] = {{"-remove\n", strcmp, RemoveFile},
	 						  {"-exit\n", strcmp, ExitProgram},
	 						  {"-count\n", strcmp, CountLines},
	 						  {"<", CompareFirstChar, AddToBeginning},
	 						  {"default",DefaultCompare, AddToEnd}};



/*******************************************************************************************************************************************/
void Print(int x)
{
	printf("my number is: %d\n\n",x);
}
struct print_me
{
	int x;
	void (*ptr)(int);
};

struct print_me struct_arr[10] = {0};

void PrintStructArr(struct print_me *arr,int size)
{
	int i = 0;
	for (i =0; i < size;i++)
	{
		arr[i].ptr(arr[i].x);
	}
}

/**********************************************************************************************************************************************/


int CompareFirstChar(const char *a, const char *b)
{
	assert(a && b);

	return (*a - *b);
}

int DefaultCompare(const char *a, const char* b)
{
	(void)a;
    (void)b;
    return SUCCESS;
}

int AppendText(char *file_name)
{

	char new_text[100] = {0};
	/*FILE *fp = NULL;*/
	int i = 0;
	int error =SUCCESS;
	assert(file_name);

	if( NULL == fgets(new_text, sizeof(new_text),stdin))
		{
			return STDIN_FAILURE;
		}

	while (!error)
	{


		for (i = 0; i < 5; ++i)
		{
			if (0 == flag_controller[i].compare(flag_controller[i].word,new_text))
			{
				error = flag_controller[i].operation(file_name,new_text);
				break;
			}
		}

		if (error)
		{
			return error;
		}

		if( NULL == fgets(new_text, sizeof(new_text),stdin))
		{
			return STDIN_FAILURE;
		}

	}

	return error;
}

void ErrorHandle(enum error_handle error)
{
	switch (error)
	{
	case SUCCESS:
        break;

    case STDIN_FAILURE:
		fprintf(stderr, "failed to read input from user");
		break;

	case FOPEN_FAIL:
		fprintf(stderr, "failed to open file"); break;

	case FILE_WRITE_FAIL:
		fprintf(stderr, "failed to add text to file"); break;

	case FCLOSE_FAIL:
		fprintf(stderr, "failed to close file"); break;

	case REMOVE_FAILURE:
		fprintf(stderr, "failed to delete file"); break;

	case RENAME_FAILURE:
		fprintf(stderr, "could NOT rename file"); break;

    }
}

int AddToEnd(char *file, char *new_text)
{
	FILE *fp = fopen(file,"a");
	if (NULL == fp)
	{
		return FOPEN_FAIL;
	}

	if (EOF == fputs(new_text,fp))
	{
		return FILE_WRITE_FAIL;
	}
	if (fclose(fp))
	{
		return FCLOSE_FAIL;
	}

	 return SUCCESS;
}

int AddToBeginning(char *old_file,char *str)
{
	FILE *new_file_p = NULL, *old_file_p = NULL;
	char *real_str = NULL;
	char buffer[200] = {0};
	char *bp = NULL;

	assert(old_file && str);

	new_file_p = fopen("newFile.txt", "a");
	old_file_p = fopen(old_file,"r");

	if (NULL == new_file_p || NULL == old_file_p)
	{
		return FOPEN_FAIL;
	}

	real_str = strcpy(str,(str+1));

	if (EOF == fputs(real_str, new_file_p))
	{
		return FILE_WRITE_FAIL;
	}

	bp = fgets(buffer, sizeof(buffer),old_file_p);
	while (bp)
	{
		if (EOF == fputs(buffer, new_file_p))
		{
			return FILE_WRITE_FAIL;
		}

		bp = fgets(buffer, sizeof(buffer),old_file_p);
	}

	if (remove(old_file))
	{
		return REMOVE_FAILURE;
	}

	old_file_p = NULL;

	if (rename("newFile.txt",old_file))
	{
		return RENAME_FAILURE;
	}

	if (fclose(new_file_p))
		{
			return FCLOSE_FAIL;
		}

	return SUCCESS;
}


int ExitProgram(char *file, char *s)
{
    (void)file;
    (void)s;
	exit(0);
}

int RemoveFile(char *file, char *s)
{
	assert(file);
    (void)s;
	if(remove(file))
	{
		return REMOVE_FAILURE;
	}
	else
	{
		return SUCCESS;
	}
}

int CountLines(char *file,char *s)
{
	int lines = 0;
	char ch = 0;
	FILE *fp = NULL;
    (void)s;

	assert(file);

	fp = fopen(file,"a+");

	if (NULL == fp)
	{
		return FOPEN_FAIL;
	}

	while ((ch = fgetc(fp)) != EOF)
    {
      if ('\n' == ch)
      {
      	++lines;
      }
    }

    if (fclose(fp))
    {
    	return FCLOSE_FAIL;
    }

    printf("there are %d lines in the file.\n",lines);
    return SUCCESS;

}


int main(int argc, char *argv[])
{
	if (2 == argc)
	{
		ErrorHandle(AppendText(argv[1]));
	}
	return 0;
}
