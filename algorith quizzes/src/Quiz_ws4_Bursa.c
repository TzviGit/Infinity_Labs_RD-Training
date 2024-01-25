#include <stddef.h>
#include <stdio.h>

typedef struct
{
    size_t profit;
    size_t buy_ind;
    size_t sell_ind;
}best_buy;

best_buy Bursa(int *arr, size_t len)
{
    best_buy bursa_max = {0};
    size_t i = 0;
    size_t buy = 0, sell = 0, temp_b = 0;

    for (i = 1; i < len; ++i)
    {
        if (arr[i] < arr[temp_b])
        {
            temp_b = arr[i];
        }
        else if ((arr[i] - arr[temp_b]) > (arr[sell] - arr[buy]))
        {
            buy = temp_b;
            sell = i;
        }
    }

    bursa_max.profit = (arr[sell] - arr[buy]);
    bursa_max.buy_ind = buy;
    bursa_max.sell_ind = sell;

    return bursa_max;

}

int main()
{
    int arr[] = {6,12,3,5,1,4,9,2};
    best_buy result = Bursa(arr, 8);
    printf("the max profit is %lu, the buying index is %lu, and the sell index is %lu\n\n",result.profit, result.buy_ind, result.sell_ind);
    return 0;
}
