/******************************************************************************
* Binary Search Tree module - Implementation C File
*
* Author: Tzvi Moskowitz
* Approved by: Fania
* Description: A non-Recursive Implementation of the standard Binary
*               Search tree. the tree is not self balncing.
*
* ILRD OL110, Ramat Gan
*******************************************************************************/


#include <assert.h> /*asserT*/
#include <stdlib.h> /*malloc*/
#include "binary_search_tree.h" /*API FOR THIS MODULE*/



#define NULL_ITER NULL



typedef enum
{
   LEFT,
   RIGHT,
   NUM_OF_CHILDREN
} children_t;


#define GET_ROOT(bst) ((bst)->dummy_iter.children[LEFT])


typedef struct bst_node
{
   void *data;
   struct bst_node *parent;
   struct bst_node *children[NUM_OF_CHILDREN];
} bst_node_t;

struct bst
{
   bst_node_t dummy_iter;
   int (*is_before)(const void *, const void *, void *param);
   void *param;
};


static int Increment(void *data, void *param);
static void RemoveWithoutIterInvalidation(bst_iter_t iter);
static int GetChildTypeOfNode(bst_iter_t iter);
static int IsSameData(void *d1, void *d2, find_func is_before, void *param);
static void AttachChild(bst_node_t*child, bst_node_t *parent, int which_son);
static void DetachChild( bst_node_t *child, bst_node_t *parent);
static bst_iter_t SubTreeMax(bst_iter_t iter);
static bst_iter_t SubTreeMin(bst_iter_t iter);
static int IsLeftSon(bst_node_t *node);
static int IsRightSon(bst_node_t *node);
static int IsRoot(bst_node_t *node);
static bst_iter_t GetOnlyChild(bst_iter_t iter);
static void FreeAndNullify(void **pointer);
static void NodeInitialize(bst_node_t *node, void *data,bst_node_t *parent,
                            bst_node_t *left_son, bst_node_t *right_son);




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~          ~~~~~~  BSTCreate  with a util func  NodeInitialize ~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bst_t *BSTCreate(find_func is_before, void *param)
{
    bst_t *tree = NULL;

    assert(is_before );

    tree = (bst_t *)malloc(sizeof(*tree));
    if (!tree)
    {
        return NULL;
    }

    NodeInitialize(&tree->dummy_iter, NULL, NULL, NULL, NULL);

    tree->is_before = is_before;
    tree->param = param;

    return tree;
}


static void NodeInitialize(bst_node_t *node, void *data,bst_node_t *parent,
                            bst_node_t *left_son, bst_node_t *right_son)
{
    node->data = data;
    node->parent = parent;
    node->children[LEFT] = left_son;
    node->children[RIGHT] = right_son;
}


/*****************************************************************************
***********      BSTDestroy    with Util_func      ***************************/


void BSTDestroy(bst_t *bst)
{
	assert(bst);

	while (!BSTIsEmpty(bst))
	{
		BSTRemove(GET_ROOT(bst));
	}


	FreeAndNullify((void *)&bst);
	return;
}


static void FreeAndNullify(void **pointer)
{
    assert(pointer);

    free((void *)(*(size_t **)pointer));
    *(size_t **)pointer = NULL;

    pointer = NULL;
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~                        ~~~~ BSTInsert    wit AttachChild util ~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bst_iter_t BSTInsert(bst_t *bst, void *data)
{
    bst_iter_t new_node = NULL_ITER;
    bst_iter_t insert_point = NULL_ITER;
    children_t which_son = 0;
    find_func is_before = NULL;
    void *param = NULL;

    assert(bst);

    is_before = bst->is_before;
    param = bst->param;
    new_node = (bst_iter_t)malloc(sizeof(*new_node));
    if (!new_node)
    {
        return &bst->dummy_iter;
    }


    insert_point = &bst->dummy_iter;
    while (insert_point->children[which_son])
    {
        insert_point = insert_point->children[which_son];
        which_son = !is_before(data, BSTGetData(insert_point), param);
    }


    NodeInitialize(new_node, data, NULL, NULL, NULL);
    AttachChild( new_node, insert_point, which_son);

    return new_node;
}


static void AttachChild(bst_node_t*child, bst_node_t *parent, int which_son)
{

    assert(parent);

    parent->children[which_son] = child;

    if (child)
    {
        child->parent = parent;
    }
}

/****************************************************************************
*****               BSTRemove with its Util_fuctions  *****************
**************************************************************************/

void *BSTRemove(bst_iter_t iter)
{
    size_t num_of_children = !!iter->children[LEFT] + !!iter->children[RIGHT];
    void *data = BSTGetData(iter);
    bst_iter_t child = NULL_ITER;
    children_t which_son = 0;


    switch (num_of_children)
    {
        case 0 :
                DetachChild(iter, iter->parent);
                break;

        case 1 :
                child = GetOnlyChild(iter);

                which_son = GetChildTypeOfNode(iter);
                AttachChild(child, iter->parent, which_son);
                break;

        case 2 :
                RemoveWithoutIterInvalidation(iter);
                break;
            }

        FreeAndNullify((void *)&iter); iter = NULL;

        return data;


}

static void RemoveWithoutIterInvalidation(bst_iter_t iter)
{
    bst_iter_t prev_node = NULL;
    bst_iter_t prev_parent = NULL;
    children_t  side_of_node = 0;
    children_t side_of_prev_node = 0;

    assert(iter);
    assert(iter->children[LEFT]);

    prev_node = BSTPrev(iter);
    prev_parent = prev_node->parent;
    side_of_node = GetChildTypeOfNode(iter);
    side_of_prev_node = GetChildTypeOfNode(prev_node);

    AttachChild(prev_node, iter->parent, side_of_node);
    AttachChild(iter->children[RIGHT], prev_node, RIGHT);

    if (prev_parent != iter->parent)
    {
        AttachChild(prev_node->children[LEFT], prev_parent, side_of_prev_node);
        AttachChild(iter->children[LEFT], prev_node, LEFT);
    }
}


static void DetachChild( bst_node_t *child, bst_node_t *parent)
{
    children_t which_son = 0;
    assert(child && parent);

    which_son = GetChildTypeOfNode(child);
    child->parent = NULL;
    parent->children[which_son] = NULL;
}


/****************   BSTIsEmpty  *************/

int BSTIsEmpty(const bst_t *bst)
{
    assert(bst);

    return !GET_ROOT(bst);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~           ~~~~ BSTSize and Util funcs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

size_t BSTSize(const bst_t *bst)
{
    size_t count = 0;
    assert(bst);

    BSTForEach( BSTBegin(bst), BSTEnd(bst), Increment, (void *)&count);

    return count;
}


static int Increment(void *data, void *param)
{
    (void)data;
    ++(*(int *)param);
    return 0;
}


/****************   BSTBegin *************/

bst_iter_t BSTBegin(const bst_t *bst)
{
    bst_iter_t curr = NULL_ITER;
    assert(bst);

    curr = (bst_iter_t)&bst->dummy_iter;

    while (curr->children[LEFT])
    {
        curr = curr->children[LEFT];

    }

    return curr;
}


/****************   BSTEnd O(1) *************/

bst_iter_t BSTEnd(const bst_t *bst)
{
    assert(bst);

    return (bst_iter_t)&bst->dummy_iter;
}


/****************   BSTNext*************/

bst_iter_t BSTNext(bst_iter_t iter)
{

    if (iter->children[RIGHT])
    {
        return SubTreeMin(iter->children[RIGHT]);
    }
    else
    {
        while (!IsRoot(iter) && !IsLeftSon(iter))
        {
            iter = iter->parent;
        }

        return iter->parent;
    }
}

/****************   BSTPrev    *************/

bst_iter_t BSTPrev(bst_iter_t iter)
{

    if (iter->children[LEFT])
    {
        return (SubTreeMax(iter->children[LEFT]));
    }
    else
    {
        while (!IsRightSon(iter))
        {
            iter = iter->parent;
        }

        return iter->parent;
    }
}

/****************   BSTGetData *************/


void *BSTGetData(bst_iter_t iter)
{
    assert(iter);

    return iter->data;
}


/****************   BSTIterIsEqual *************/

int BSTIterIsEqual(bst_iter_t iter1, bst_iter_t iter2)
{
    return iter1 ==iter2;
}


/****************   BSTFind *************/

bst_iter_t BSTFind(const bst_t *bst, void *data)
{
    bst_iter_t iter = NULL;
    children_t which_son = 0;
    find_func is_before = NULL;
    void *param = NULL;

    assert(bst);
    assert(data);

    is_before = (find_func)bst->is_before;
    param = (void *)bst->param;

    iter = (bst_iter_t)&bst->dummy_iter;
    while (iter->children[which_son])
    {
        iter = iter->children[which_son];

        if (IsSameData(data, BSTGetData(iter), is_before, param))
        {
            break;
        }

        which_son = !is_before(data, BSTGetData(iter), param);
    }

    return IsSameData(data, BSTGetData(iter), is_before, param) ?
                                    iter : (bst_iter_t)&bst->dummy_iter;

}

/****************   BSTForEach *************/


int BSTForEach(bst_iter_t from, bst_iter_t to,
			   action_func action, void *param)
{
    bst_iter_t iter = NULL;

    assert(action );

    for (iter = from; !BSTIterIsEqual(iter, to); iter = BSTNext(iter))
    {
        if (action(BSTGetData(iter),param))
        {
            return 1;
        }
    }

    return 0;

}


/*******************************************************************************
**        ~~~~~~  GENERAL PURPOSE UTILITY FUNCTIONS    ~~~~~
****************************************************************************/

static int GetChildTypeOfNode(bst_iter_t iter)
{
    return (iter->parent->children[RIGHT] == iter);
}


static int IsRoot(bst_iter_t iter)
{
    return (NULL == iter->parent->parent);
}


static int IsLeftSon(bst_node_t *node)
{
    return (node->parent->children[LEFT] == node);
}

static int IsRightSon(bst_node_t *node)
{
    return (node->parent->children[RIGHT] == node);
}

static bst_iter_t SubTreeMax(bst_iter_t iter)
{
    while (iter->children[RIGHT])
    {
        iter = iter->children[RIGHT];
    }

    return iter;
}


static bst_iter_t SubTreeMin(bst_iter_t iter)
{
    while (iter->children[LEFT])
    {
        iter = iter->children[LEFT];
    }

    return iter;
}




static int IsSameData(void *d1, void *d2, find_func is_before, void *param)
{
    int bool1 = !is_before(d1, d2, param);
    int bool2 = !is_before(d2, d1, param);

    return (bool1 && bool2);
}





static bst_iter_t GetOnlyChild(bst_iter_t iter)
{
    return iter->children[!(iter->children[LEFT])];
}
