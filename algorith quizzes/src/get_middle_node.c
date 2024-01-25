#include <assert.h>
#include <stdlib.h>
#include "test_util.h"

typedef struct node node_t;

static node_t *CreateList(size_t n_nodes);

struct node
{
    void *data;
    node_t *next;
};

/***************************************************************************
*   HERES THE ACTUAL ALGORITH...  TIME_COMPLX: O(n)    SPACE_COMPLX: O(1)
**************************************************************************/
node_t *GetMiddleNode(node_t *head)
{
    node_t *node_ptr = head;
    size_t count = 0;
    assert(head);

    /****FIND LENGTH OF LIST******/
    for (node_ptr = head; (node_ptr); node_ptr = node_ptr->next)
    {
        ++count;
    }

    /****FOR EXAMP. IF THERE ARE 4 NODES WE RETURNS THE 2nd.******/
    count = (count / 2) - !(count % 2);

    for ( ; count > 0;  --count)
    {
        node_ptr = node_ptr->next;
    }

    return node_ptr;
}



/***********==== TEST FUNCTION*****/
void TestGetMiddleNode(void)
{
    int flag = 0;
    node_t *list_head_even = CreateList(10);
    node_t *list_head_odd = CreateList(7);

    VERIFY((size_t)5 == (size_t)GetMiddleNode(list_head_even)->data);
    VERIFY((size_t)4 == (size_t)GetMiddleNode(list_head_odd)->data);

    CHECK_SUCCESS(flag, GET_MIDDLE_NODE);

    free(list_head_odd); list_head_odd = NULL;
    free(list_head_even); list_head_even = NULL;
}

int main()
{
    TestGetMiddleNode();
    return 0;
}


/***************************************************************************
*       UTILITY FUNC: INITIALIZE List with given length, setting their
*       respective data fiels to their placement in the list .
**************************************************************************/
static node_t *CreateList(size_t n_nodes)
{
    size_t i = 0;
    node_t *pool = (node_t *)malloc(n_nodes * sizeof(*pool));
    node_t *init_ptr = pool;
    if (!pool)
    {
        return NULL;
    }

    for (i = 1; i < n_nodes; ++i)
    {
        init_ptr->data = (void *)i;
        init_ptr->next = (init_ptr + 1);
        ++init_ptr;
    }

    init_ptr->data = (void *)i;
    init_ptr->next = NULL;

    return pool;

}
