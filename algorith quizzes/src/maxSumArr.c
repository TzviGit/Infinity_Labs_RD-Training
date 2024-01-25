#include <stddef.h>
#include <stdio.h>


struct max_sub
{
    int maxSum ;
    int f_ind ;
    int l_ind ;
}test_arr, *max_arr;

struct max_sub *MaxSumArr(int *arr, size_t length, struct max_sub *max_arr)
{
    int current_sum = 0;
    size_t i = 0;

    while (arr[i] < 0)
    {
        ++i;
    }
    max_arr->f_ind = i;
    current_sum = arr[i];
    for (++i ; i <length; ++i)
    {
        if (arr[i] < 0)
        {
            max_arr->maxSum = current_sum;
            max_arr->l_ind = i - 1;
            current_sum += arr[i];
            if (current_sum   < 0)
            {
                current_sum = 0;
                max_arr->f_ind = max_arr->l_ind = i + 1;

            }
        }
        else
        {
            current_sum += arr[i];
            if (current_sum >= max_arr->maxSum)
            {
                max_arr->maxSum = current_sum;
                max_arr->l_ind = i;
            }
        }
    }
        return max_arr;
}
int main()
{
    int arr[10] = {-1,1000,0,-3,4,5,6,-3,-45,1000};
    max_arr = &test_arr;
    max_arr = MaxSumArr(arr,10,max_arr);
    printf("Max sum is %d, and the indexes are from %d till %d.\n",max_arr->maxSum,max_arr->f_ind, max_arr->l_ind);

    return 0;
}
