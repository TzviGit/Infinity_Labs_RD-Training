/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      --------------
* Description   heuristic Backtracking solution to the knights tour problem
* Date:         November 3, 2021
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <assert.h> /* assert */
#include <stddef.h> /*size_t*/
#include "knight_tour.h" /*API for this module*/
#include "bit_array.h"  /* API for Bit_Array DS used in our imple */
#include <limits.h> /*INT_MAX*/



/***************************************************************************
*		enum of the possible jumps the knight can take.
*       (idea credit: -Tuvia Reznik)
***************************************************************************/
typedef enum {
	LEFT_DOWN_DOWN,
	LEFT_LEFT_DOWN,
	LEFT_LEFT_UP,
	LEFT_UP_UP,
	RIGHT_UP_UP,
	RIGHT_RIGHT_UP,
	RIGHT_RIGHT_DOWN,
	RIGHT_DOWN_DOWN,
	NUM_OF_JUMP_TYPES
} jump_t;




static jump_t PopBestNextPath(int *jump_proir_table);
static void FillJumpPriorityTable(int *jump_proir_table, size_t rows,
	 						size_t cols, size_t curr_ind, bit_arr_t bit_LUT);

static int KnightTourImple(size_t **final_path, size_t rows, size_t cols,
										size_t curr_ind, bit_arr_t bit_LUT);
static int IsValidJump(size_t jump_ind, bit_arr_t bit_LUT);

static size_t IndexOfJump(size_t rows, size_t cols, size_t curr, jump_t dir);
static int IsValidJump(size_t jump_ind, bit_arr_t bit_LUT);





/*^^^^^^^^^^^^^^         UTIL_FUNCTIONS  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/***************************************************************************
*    ~~~~~~  calculates the index after jump in direction "dir"  ~~~
*                  or (size_t) -1 if jump is out of bounds.
*
*	REMEMBER-- The board is represented as a two dimentional array as follows:
*
*   	 x\y     col[0]  col[2]  col[3] ...
*    	row[0]  b[0][0] b[0][1] b[0][2]
*    	row[1]  b[1][0] b[1][1] b[1][2]
*    	row[2]  b[2][0] b[2][1] b[2][2]
*    	...
*			(PHOTO CREDIT: VADIM LUKASHEVICH)
 *****************************************************************************/


static size_t IndexOfJump(size_t rows, size_t cols, size_t curr, jump_t dir)
{
	switch (dir)
	{
		case LEFT_DOWN_DOWN:
			return ((curr / cols > 1) && (curr % cols > 0)) ?
			(curr - (cols * 2) - 1) : (size_t)-1;

		case LEFT_LEFT_DOWN:
			return ((curr / cols > 0) && (curr % cols > 1)) ?
			(curr - (cols * 1) - 2) : (size_t)-1;

		case LEFT_LEFT_UP:
			return ((curr / cols < rows - 1) && (curr % cols > 1)) ?
			(curr + (cols * 1) - 2) : (size_t)-1;

		case LEFT_UP_UP:
			return ((curr / cols < rows - 2) && (curr % cols > 0)) ?
			(curr + (cols * 2) - 1) : (size_t)-1;

		case RIGHT_UP_UP:
			return ((curr / cols < rows - 2) && (curr % cols < cols - 1)) ?
			(curr + (cols * 2) + 1) : (size_t)-1;

		case RIGHT_RIGHT_UP:
			return ((curr / cols < rows - 1) && (curr % cols < cols - 2)) ?
			(curr + (cols * 1) + 2) : (size_t)-1;

		case RIGHT_RIGHT_DOWN:
			return ((curr / cols > 0) && (curr % cols < cols - 2)) ?
			(curr - (cols * 1) + 2) : (size_t)-1;

		case RIGHT_DOWN_DOWN:
			return ((curr / cols > 1) && (curr % cols < cols - 1)) ?
			(curr - (cols * 2) + 1) : (size_t)-1;

		default:
			return (size_t)-1;

	}
}

/***************************************************************************
   		Boolean indicator that jump to given index is valid.
	the "jump_ind" MUST be the return val of the "IndexOfJump" function
*****************************************************************************/

static int IsValidJump(size_t jump_ind, bit_arr_t bit_LUT)
{
    return (size_t)-1 != jump_ind && 0 == GetBit(bit_LUT, jump_ind);
}


/*****************************************************************************
*  the KnightTour func with is recursive Imle func and helper functions
*****************************************************************************/

int KnightTour(size_t **final_path, size_t rows, size_t cols,
                size_t start_x, size_t start_y)
{
    size_t start_index = (cols * start_x) + start_y;
	bit_arr_t bit_LUT = 0;

    return KnightTourImple(final_path, rows, cols, start_index, bit_LUT);
}


static int KnightTourImple(size_t **final_path, size_t rows, size_t cols,
							   size_t curr_ind, bit_arr_t bit_LUT)
{
    size_t i = 0;
    int status = 1;
    size_t step_count = 0;
    int jump_proir_table[NUM_OF_JUMP_TYPES] = {0};


    bit_LUT = SetOn(bit_LUT, curr_ind);

    step_count = CountOn(bit_LUT);


    if ((rows * cols) == step_count)
    {
        final_path[curr_ind / cols][curr_ind % cols] = step_count;
        return 0;
    }

    FillJumpPriorityTable(jump_proir_table, rows, cols, curr_ind, bit_LUT);


    for (i = 0; i < NUM_OF_JUMP_TYPES; ++i)
    {
        size_t jump_ind = 0;
        jump_t next_dir = PopBestNextPath(jump_proir_table);

        if (NUM_OF_JUMP_TYPES == next_dir)
        {
            break;
        }

        jump_ind = IndexOfJump(rows, cols, curr_ind, next_dir);
        status = KnightTourImple(final_path, rows, cols, jump_ind, bit_LUT);

        if (0 == status)
        {
            final_path[curr_ind / cols][curr_ind % cols] = step_count;

            return 0;
        }
    }

    return 1;


}


/*************************************************************************
*     	This functions properly fills the table with each  cell index
		representing a jump_t, and the value in the cell is the
		number of valid sub-paths available after landing from a
		jump in the given jump_t.
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void FillJumpPriorityTable(int *jump_proir_table, size_t rows,
	 				  size_t cols, size_t curr_ind, bit_arr_t bit_LUT)
{
    size_t i = 0;

    for (i = 0; i < NUM_OF_JUMP_TYPES; ++i)
    {
        int count = -1;
        size_t index = IndexOfJump(rows, cols, curr_ind, i);

        if (IsValidJump(index, bit_LUT))
        {
			jump_t j = 0;
            for (j = 0; j < NUM_OF_JUMP_TYPES; ++j)
            {
                if ((size_t)-1 != IndexOfJump(rows, cols, index, j))
                {
                    ++count;
                }
            }

            jump_proir_table[i] = count;
        }

    }

}

/*************************************************************************
*     		This functions implemts the Heuristic aproach--
* 	--always taking the path with the least number of visible valid paths
*		also know as Wornsdorff's algorith
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static jump_t PopBestNextPath(int *jump_proir_table)
{

    jump_t best_direction = NUM_OF_JUMP_TYPES;
    size_t i = 0;
    int min = INT_MAX;

    for (i = 0; i < NUM_OF_JUMP_TYPES; ++i)
    {
        if (jump_proir_table[i] != 0 && jump_proir_table[i] < min)
        {
			min = jump_proir_table[i];
            best_direction = (jump_t)i;
            jump_proir_table[i] = 0;
		}
    }

    return best_direction;
}
