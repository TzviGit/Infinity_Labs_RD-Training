#include "test_util.h"
#include "knight_tour.h"
#include <stdlib.h>
int rows = 8;
int cols = 8;
void PrintResult(size_t **array)
{
    size_t i,j;

    for (i = 0; i < (size_t)rows; ++i)
    {
        for (j = 0; j < (size_t)cols; ++j)
        {
            printf(" [%lu] ",array[i][j]);
        }

        printf("\n\n" );
    }
}

void TestGame(void)
{
    RESET_FLAG;
    size_t i = 0, j = 0, k = 0;   int found = 0;
    size_t **final_path = (size_t **)malloc(rows * sizeof(size_t*));
    for (i = 0; i < (size_t)rows; i++)
	{
		final_path[i] = (size_t *)calloc(cols, sizeof(size_t));
	}

    for (k = 0; k < 1; ++k)
    {
        for (i = 0; i < rows; ++i)
        {
            for (j = 0; j < cols; ++j)
            {
                VERIFY(0 == KnightTour(final_path, rows, cols, i , j));
            }
        }
    }



    VERIFY(0 == found);

    PrintResult(final_path);

    CHECK_SUCCESS(Chess);

    for (i = 0; i < (size_t)rows; i++)
	{
		free(final_path[i]);
	}

    free(final_path);
}




int main()
{
    TestGame();
    return 0;
}
