#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H
#include <stddef.h> /* size_t */

typedef size_t bit_arr_t; /* supports word sized bit arrays */

/****************** FUNCTIONS ******************************/

bit_arr_t SetAll(bit_arr_t arr); /* turns all bits on */

bit_arr_t ResetAll(bit_arr_t arr); /* turns all bits off */

bit_arr_t SetOn(bit_arr_t arr, size_t index); /* turns on a specified bit */

bit_arr_t SetOff(bit_arr_t arr, size_t index); /* turns off a specified bit */

bit_arr_t SetBit(bit_arr_t arr, size_t index, int set_as);

int GetBit(bit_arr_t arr, size_t index);

bit_arr_t FlipBit(bit_arr_t arr, size_t index);

bit_arr_t Mirror(bit_arr_t arr);
bit_arr_t RotR(bit_arr_t arr, size_t amount);

bit_arr_t RotL(bit_arr_t arr, size_t amount);

size_t CountOn(bit_arr_t arr);

size_t CountOff(bit_arr_t arr);

char *ToString(bit_arr_t arr, char *buf);

#endif /*not defined bit_array.h*/
