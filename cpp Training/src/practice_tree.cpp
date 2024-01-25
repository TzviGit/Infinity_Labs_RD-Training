#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

void dir(char *directory,char *parent);

int main(int argc, char *argv[])
{
	char current[PATH_MAX],parent[PATH_MAX];

	puts("Finding Directories");

	/* Change to or set current directory */
	if(argc==2)
	{
		strcpy(current,argv[1]);
		chdir(current);
	}
	else
	{
		getcwd(current,PATH_MAX);
	}

	/* fetch parent directory */
	chdir("..");
	getcwd(parent,PATH_MAX);
	/* and change back */
	chdir(current);
	/* go! */
	dir(current,parent);

	return(0);
}

void dir(char *directory,char *parent)
{
	DIR *folder;
	struct dirent *entry;
	struct stat filestat;
	char pathname[PATH_MAX];

	/* Change to the named directory */
	if(chdir(directory))
	{
		fprintf(stderr,"Error changing to %s\n",directory);
		exit(1);
	}

	/* open the directory */
	folder = opendir(".");
	if(folder == NULL)
	{
		fprintf(stderr,"Unable to read directory %s\n",directory);
		exit(1);
	}

	/* fetch the current directory for output, but also for
	   the recursive call later */
	getcwd(pathname,PATH_MAX);
	printf("%s\n",pathname);

	/* Look for a subdirectory */
	while( (entry=readdir(folder)) )
	{
		stat(entry->d_name,&filestat);
		/* look for only directories */
		if( S_ISDIR(filestat.st_mode) )
		{
			/* skip the . and .. entries */
			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
				continue;
			/* recurse to the found directory */
			dir(entry->d_name,pathname);
		}
	}

	closedir(folder);
	chdir(parent);
}
