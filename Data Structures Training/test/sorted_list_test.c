#include "dlist.h"
#include "sorted_list.h"



int main()
{

    sr_list_t *list = SortedLCreate(NULL);
    SortedLDestroy(list);
    return 0;
}
