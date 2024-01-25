#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include <stdlib.h> /* atoi */


void CountPathsImple(int *options, size_t size, int steps, size_t *total);
size_t CountPaths(int *options, size_t size, int steps)
{
    size_t total_paths = 0;

    CountPathsImple(options,size, steps, &total_paths);

    return total_paths;
}

void CountPathsImple(int *options, size_t size, int steps, size_t *total)
{
    size_t i = 0;
    if (0 == steps)
    {
        ++(*total);
        return;
    }

    for (i = 0; i < size; ++i)
    {
        if (options[i] <= steps)
        {
            CountPathsImple(options, size, (steps - options[i]), total);
        }
    }
}

#define MAX (100)

int main (int argc, const char *argv[])
{
    int i = 0;
    size_t size = argc > 1 ? atoi(argv[1]) : 3;
    const int steps = argc > 2 ? atoi(argv[2]) :10;
    int options[MAX] = {0};
    size_t paths = 0;
    for (i = 0; i < MAX; ++i)
    {
        options[i] = 1 + i;
    }

    paths = CountPaths(options, size, steps);

    printf("how many paths to climb %d steps with the given options 1 through %lu -->  %lu\n", steps, size, paths);

    return 0;
}












