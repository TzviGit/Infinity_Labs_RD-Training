#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_SHARED | MAP_ANONYMOUS;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, -1, 0);
}

int parent_id;

int main() 
{
    parent_id = getpid();
    char parent_message[] = "hello";  // parent process will write this message
    char child_message[] = "goodbye"; // child process will then write this one

    char *shmem = (char *)create_shared_memory(128);

    memcpy(shmem, parent_message, sizeof(parent_message));

    for (int i = 0; i < 4; ++i)
    {
        int pid = fork();

        if (pid == 0)
        {
            while(parent_id == getppid());

            printf("Child # %d read: %s\n", getpid(), shmem);
            // memcpy(shmem, child_message, sizeof(child_message));
            // printf("Child wrote: %s\n", shmem);

            exit(0);
        }
  }
  
  
    printf("Parent wrote: %s\n", shmem);
    sleep(1);
    printf("parent exiting .... \n");

    exit(0);
}