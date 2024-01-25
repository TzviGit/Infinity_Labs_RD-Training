/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      Shany
* Description   AVL RECURSIVE TREE Implementation
* Date          October 19, 2021
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/


#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc ,free*/
#include "avl_tree.h" /*API for this file (AVL RECURSIVE TREE) */


int MyCompare(const void *d1, const void *d2);


typedef enum
{
    RIGHT_SKEWED = -2,
    LEFT_SKEWED = 2
}skew_t;

typedef enum
{
    LEFT,
    RIGHT,
    UNDEFINED,
    NUM_OF_CHILDREN
} children_t;

typedef struct avl_node
{
    void *data;
    long height;
    struct avl_node *children[NUM_OF_CHILDREN];
}avl_node_t;

struct avl
{
    int (*compare)(const void *, const void *);
    avl_node_t *root;
};




#define WILDCARD_CHILD(node) (!!(node)->children[RIGHT])

#define GET_ONLY_CHILD_IF_EXISTS(node) (HAS_NO_CHILDREN((node)) ? NULL : \
                    (node)->children[WILDCARD_CHILD((node))])

#define HAS_NO_CHILDREN(node) (!((node)->children[LEFT]) \
                        && !((node)->children[RIGHT]))

#define HAS_ONE_CHILD(node) (1 == !!((node)->children[LEFT])+ \
                            !!((node)->children[RIGHT]))



long RecSubTreeHeight(avl_node_t *node);
int IsBalanced(avl_node_t *node);

#define LEFT_SON(node) ((node)->children[LEFT])
#define RIGHT_SON(node) ((node)->children[RIGHT])
#define GET_SON(node, direction) ((node)->children[(direction)])


typedef int (*compare_t)(const void *, const void *);
typedef int (*action_t)(void *data, void *param);




static int ActionTraverseInOrder(avl_node_t *node, action_t action, void *param);
static int ActionTraversePreOrder(avl_node_t *node, action_t action, void *param);
static int ActionTraversePostOrder(avl_node_t *node, action_t action, void *param);

static int IncrementCount(void *data, void *param);
static avl_node_t *AVLFindNodeImple(avl_node_t *node,
            const void *to_find, compare_t compare);

static void AVLDestroySubTree(avl_node_t *node);

static void FreeAndNullFields(avl_node_t **node);
static void UpdateNodeHeight(avl_node_t *node);
static long GetHeight(avl_node_t *node);

static avl_node_t *AVLInsertImpl(avl_node_t **node, avl_node_t *new_node,
                                                    compare_t compare);


static avl_node_t *AVLRemoveImple(avl_node_t *node,
        const void *key_to_remove, compare_t compare, void **removed_data);

static avl_node_t *RemoveNode(avl_node_t *node, compare_t compare);
avl_node_t *GetMaxSubTree(avl_node_t *root);
static avl_node_t *AvlCreateNode(const void *data, size_t height);
static avl_node_t *ReBalance(avl_node_t *node);
static avl_node_t *RotateLeft(avl_node_t *node);
static avl_node_t *RotateRight(avl_node_t *node);
static int GetBalanceFactor(avl_node_t *node);



static long MAX(long l1, long l2)
{
    return l1 >= l2 ? l1 : l2;
}








/**************************************************************************
*          ~~~~~~~~~~    AVL_CREATE    ~~~~~~~~
***************************************************************************/

avl_t *AVLCreate(int (*compare)(const void *d1, const void *d2))
{
    avl_t *avl = (avl_t *)malloc(sizeof(*avl));
    if (NULL == avl)
    {
        return NULL;
    }

    assert(compare);

    avl->compare = compare;
    avl->root = NULL;

    return avl;
}



/**************************************************************************
*          ~~~~~~~~~~    AVL_DESTROY    ~~~~~~~~
***************************************************************************/



void AVLDestroy(avl_t *avl)
{
    assert(avl);

    AVLDestroySubTree(avl->root);

    avl->root = NULL;
    avl->compare = NULL;

    free(avl);
    avl = NULL;

}

static void AVLDestroySubTree(avl_node_t *node)
{
    if (NULL == node)
    {
        return;
    }

    AVLDestroySubTree(LEFT_SON(node));
    AVLDestroySubTree(RIGHT_SON(node));

    FreeAndNullFields(&node);
}


static void FreeAndNullFields(avl_node_t **node)
{
    assert(node);

    LEFT_SON(*node) = NULL;
    RIGHT_SON(*node) = NULL;

    free(*node);
    *node = NULL;

}

/****************************************************************************
*      ~~~~~ THE BALANCING FUNCTIONS - "THE MOTOR" OF THE AVL_TREE ~~~~~~~~
*  ~~ Notice How I use only Two Rotate Functions to  account for all
*  the  possibilities. ~~~~
***************************************************************************/

static avl_node_t *ReBalance(avl_node_t *node)
{
    int balance_factor = GetBalanceFactor(node);

    if (balance_factor == LEFT_SKEWED)
    {
        if (GetBalanceFactor(LEFT_SON(node)) < 0)
        {
            LEFT_SON(node) = RotateLeft(LEFT_SON(node));
        }
        return RotateRight(node);
    }
    else if (balance_factor == RIGHT_SKEWED)
    {
        if (GetBalanceFactor(RIGHT_SON(node)) > 0)
        {
            RIGHT_SON(node) = RotateRight(RIGHT_SON(node));
        }
        return RotateLeft(node);
    }

    return node;
}

static avl_node_t *RotateLeft(avl_node_t *node)
{
    avl_node_t *go_left = node;
    avl_node_t *go_top = RIGHT_SON(node);

    RIGHT_SON(go_left) = LEFT_SON(go_top);
    LEFT_SON(go_top) = go_left;

    UpdateNodeHeight(go_left);
    UpdateNodeHeight(go_top);

    return go_top;
}

static avl_node_t *RotateRight(avl_node_t *node)
{
    avl_node_t *go_right = node;
    avl_node_t *go_top = LEFT_SON(node);

    LEFT_SON(go_right) = RIGHT_SON(go_top);
    RIGHT_SON(go_top) = go_right;

    UpdateNodeHeight(go_right);
    UpdateNodeHeight(go_top);

    return go_top;
}


static int GetBalanceFactor(avl_node_t *node)
{
    return (GetHeight(LEFT_SON(node)) - GetHeight(RIGHT_SON(node)));
}


/*******************   UPDATE_HEIGHT  FUNCS ********************************/

static void UpdateNodeHeight(avl_node_t *node)
{
    node->height =
            (1 +  MAX(GetHeight(LEFT_SON(node)), GetHeight(RIGHT_SON(node))));
}


static long GetHeight(avl_node_t *node)
{
    if (node)
    {
        return node->height;
    }

    return  (-1);
}


/**************************************************************************
*    ~~~~~~~~~ THE INSERT FUNCS AND THEIR UTILITY FUNCTIONS ~~~~~~~~~
*************************************************************************/

int AVLInsert(avl_t *avl, const void *data)
{
    avl_node_t *new_node = AvlCreateNode(data, 0);
    if (NULL == new_node)
    {
        return 1;
    }

    assert(avl);
    assert(data);

    avl->root = AVLInsertImpl(&avl->root, new_node, avl->compare);


    return 0;
}


static avl_node_t *AVLInsertImpl(avl_node_t **curr_node,
                                   avl_node_t *new_node, compare_t compare)
{
    void *data = new_node->data;

    children_t direction = 0;
    assert(curr_node);
    assert(compare);

    if (NULL == *curr_node)
    {
        return new_node;
    }

    direction = (compare(data, (*curr_node)->data) >= 0) ? RIGHT : LEFT;


    GET_SON(*curr_node, direction) =
    AVLInsertImpl(&(*curr_node)->children[direction], new_node, compare);



    UpdateNodeHeight(*curr_node);
    *curr_node = ReBalance(*curr_node);
    return *curr_node;

}




static avl_node_t *AvlCreateNode(const void *data, size_t height)
{
    avl_node_t *new_node = (avl_node_t *)malloc(sizeof(*new_node));
    if (NULL == new_node)
    {
        return NULL;
    }

    new_node->data = (void *)data;
    new_node->height = height;

    LEFT_SON(new_node) = NULL;
    RIGHT_SON(new_node) = NULL;

    return new_node;
}

/*****************************************************************************
*  ------------  AVL_REMOVE AND ALL ITS Imple FUNCS ------------
*
*****************************************************************************/


void *AVLRemove(avl_t *avl, const void *key)
{
    void *removed_data = NULL;

    assert(avl);


    avl->root = AVLRemoveImple(avl->root, key, avl->compare, &removed_data);


    return removed_data;
}


static avl_node_t *AVLRemoveImple(avl_node_t *node,
        const void *key_to_remove, compare_t compare, void **removed_data)
{

    int direction = 0;
    int compare_result = 0;

    if (NULL == node)
    {
        return NULL;
    }

    compare_result = compare(key_to_remove, node->data);

    if (0 == compare_result)
    {
        *removed_data = node->data;

        return RemoveNode(node, compare);
    }
    else
    {
        direction = (compare_result >= 0) ? RIGHT : LEFT;
        GET_SON(node, direction) = AVLRemoveImple(GET_SON(node, direction),
                                        key_to_remove, compare, removed_data);

    }

    UpdateNodeHeight(node);
    node = ReBalance(node);

    return node;

}


static avl_node_t *RemoveNode(avl_node_t *node, compare_t compare)
{
    if (HAS_ONE_CHILD(node) || HAS_NO_CHILDREN(node))
    {
        avl_node_t *child = GET_ONLY_CHILD_IF_EXISTS(node);

        FreeAndNullFields(&node); node = NULL;

        return child;
    }
    else
    {
        avl_node_t *max_left_subtree = GetMaxSubTree(LEFT_SON(node));
        void *new_data = max_left_subtree->data;
        void *dummy = NULL;

        node->data = new_data;

        LEFT_SON(node) = AVLRemoveImple(LEFT_SON(node),
                                                new_data, compare, &dummy);

        UpdateNodeHeight(node);
        node = ReBalance(node);

        return node;
    }
}


avl_node_t *GetMaxSubTree(avl_node_t *root)
{
    if (NULL == RIGHT_SON(root))
    {
        return root;
    }

    return GetMaxSubTree(RIGHT_SON(root));
}


/* =================   AVL_IS_EMPTY  ==========================*/


int AVLIsEmpty(const avl_t *avl)
{
    assert(avl);

    return (NULL == avl->root);
}


/* ======  AVL_SIZE  with an action function IncrementCount ================*/


size_t AVLSize(const avl_t *avl)
{
    size_t count = 0;

    assert(avl);

    AVLForEach((avl_t *)avl, IncrementCount, &count, PRE);

    return count;
}


static int IncrementCount(void *data, void *param)
{
    (void)data;

    assert (param);

    ++(*(size_t *)param);

    return 0;
}

/* =================   AVL_HEIGHT  ==========================*/


int AVLHeight(const avl_t *avl)
{
    assert(avl);

    return GetHeight(avl->root);
}



/* =================   AVL_FIND   and  its UTIL_FUNCS ====================*/


void *AVLFind(const avl_t *avl, const void *to_find)
{
    avl_node_t *found_node = NULL;

    assert(avl);
    assert(to_find);

    found_node =  AVLFindNodeImple(avl->root, to_find , avl->compare);

    return (NULL != found_node) ? found_node->data : NULL;
    }

static avl_node_t *AVLFindNodeImple(avl_node_t *node,
                                        const void *to_find, compare_t compare)
{
    int comp_result = 0;

    if (NULL == node)
    {
        return NULL;
    }

    comp_result = compare(to_find, node->data);

    if (0 == comp_result)
    {
        return node;
    }

    return AVLFindNodeImple(node->children[(comp_result > 0)], to_find, compare);
}


/****************************************************************************
*     ~~~~~~~~~~ AVL_FOR_EACH and its UTIL_FUNCS  ~~~~~~~~~~~~
****************************************************************************/


int AVLForEach(avl_t *avl, action_t action, void *param, order_t order)
{
    assert(avl);
    assert(action);



    switch (order)
    {
        case PRE:
                      return ActionTraversePreOrder(avl->root, action, param);

        case IN:
                      return ActionTraverseInOrder(avl->root, action, param);

        case POST:
                      return ActionTraversePostOrder(avl->root, action, param);


        default :
                return 1;

    }

}

static int ActionTraverseInOrder(avl_node_t *node, action_t action, void *param)
{
    int flag = 0;

    if (NULL == node)
    {
        return 0;
    }

    flag = ActionTraverseInOrder(LEFT_SON(node), action, param);

    if (0 == flag)
    {
        flag = action(node->data, param);
    }

    if (0 == flag)
    {
        flag = ActionTraverseInOrder(RIGHT_SON(node), action, param);
    }

    return flag;
}


static int ActionTraversePreOrder(avl_node_t *node, action_t action, void *param)
{
    int flag = 0;

    if (NULL == node)
    {
        return 0;
    }

    flag = action(node->data, param);

    if (0 == flag)
    {
        flag = ActionTraversePreOrder(LEFT_SON(node), action, param);
    }

    if (0 == flag)
    {
        flag = ActionTraversePreOrder(RIGHT_SON(node), action, param);
    }

    return flag;
}

static int ActionTraversePostOrder(avl_node_t *node, action_t action, void *param)
{
    int flag = 0;

    if (NULL == node)
    {
        return 0;
    }

    flag = ActionTraversePostOrder(LEFT_SON(node), action, param);

    if (0 == flag)
    {
        flag = ActionTraversePostOrder(RIGHT_SON(node), action, param);
    }

    if (0 == flag)
    {
        flag = action(node->data, param);
    }

    return flag;
}



/*       <<<<<<  END OF API FUNCTIONS   >>>>>>>      */









/**************************************************************************
*       the following are just functions to help with TestToString
        not critical to the API functions
*
****************************************************************************/




#ifndef NDEBUG



#include <stdio.h>
#include "test_util.h"
static void PrintSubTree(avl_node_t *tree);
static int PrintTreeImpl(avl_node_t *tree, int is_left,
int offset, int depth, char s[7][255]);



static int ForEachNodeOfTreeImple(avl_node_t *node, action_t action, void *param)
{
    int flag = 0;

    if (NULL == node)
    {
        return 0;
    }

    flag = ForEachNodeOfTreeImple(LEFT_SON(node), action, param);

    if (0 == flag)
    {
        flag = action(node, param);
    }

    if (0 == flag)
    {
        flag = ForEachNodeOfTreeImple(RIGHT_SON(node), action, param);
    }

    return flag;
}

int ForEachNodeOfTree(avl_t *avl, action_t action, void *param)
{
    return ForEachNodeOfTreeImple(avl->root, action, param);
}





long RecSubTreeHeight(avl_node_t *node)
{
    if (!node)
    {
        return -1;
    }

    return (1 + MAX(RecSubTreeHeight(LEFT_SON(node)), RecSubTreeHeight(RIGHT_SON(node))));
}

int IsBalanced(avl_node_t *node)
{
    long h_right, h_left , difference;

    if (HAS_NO_CHILDREN(node))
    {
        return 1;
    }

    h_left = RecSubTreeHeight(LEFT_SON(node));
    h_right = RecSubTreeHeight(RIGHT_SON(node));
    difference = h_left - h_right;

    return  (difference <= 1 && difference >= (-1));
}



void PrintTree(avl_t *avl)
{
    PrintSubTree(avl->root);
}

static void PrintSubTree(avl_node_t *tree)
{
    char s[7][255] = {0};
    int i = 0;

    for (i = 0; i < 7; i++)
    {
        sprintf(s[i], "%105s", " ");
    }

    PrintTreeImpl(tree, 0, 0, 0, s);

    for (i = 0; i < 7; i++)
    {
        printf("%s\n", s[i]);
    }
}

static int PrintTreeImpl(avl_node_t *tree, int is_left,
                        int offset, int depth, char s[7][255])
{
    char b[6];
    int width = 5;
    int i = 0, left = 0, right = 0;

    if (NULL == tree)
    {
        return 0;
    }

    sprintf(b, "(%03d)", *(int*)tree->data);
    /*sprintf(b, "(%03d)", tree->height);*/
    left  = PrintTreeImpl(LEFT_SON(tree),  1, offset,                depth + 1, s);
    right = PrintTreeImpl(RIGHT_SON(tree), 0, offset + left + width, depth + 1, s);

    for (i = 0; i < width; i++)
    {
        s[depth][offset + left + i] = b[i];
    }

    if (depth != 0 && is_left)
    {
        for (i = 0; i < width + right; i++)
        {
            s[depth - 1][offset + left + (width / 2) + i] = '-';
        }

        s[depth - 1][offset + left + (width / 2)] = '.';
    }
    else if (depth != 0 && !is_left)
    {
        for (i = 0; i < left + width; i++)
        {
            s[depth - 1][offset - (width / 2) + i] = '-';
        }

        s[depth - 1][offset + left + (width / 2)] = '.';
    }

    return left + width + right;
}



int PrintNode(void *data, void *param)
{
    avl_node_t *node = (avl_node_t *)data;
    (void)param;
    printf(_CYAN_"%d"_GREEN_ "->" _WHITE_, *(int *)node->data);
    return 0;
    }

    int PutInIntArray(void *data, void *param)
    {
    static long i = 0;

    int *arr_ptr = (int *)param;

    /*printf(_CYAN_"%d"_GREEN_ "->" _WHITE_, *(int *)node->data);*/
    arr_ptr[i] = (long)data;


    ++i;
    return 0;
}

int PutIntoArray(void *data, void *param)
{
    static long i = 0;
    avl_node_t *node = (avl_node_t *)data;
    int *arr_ptr = (int *)param;

    /*printf(_CYAN_"%d"_GREEN_ "->" _WHITE_, *(int *)node->data);*/
    arr_ptr[i] = (long)node->data;


    ++i;
    return 0;
}

int VerifyHeightAndBalance(void *data, void *param)
{
    int flag = 0;
    avl_node_t *node = (avl_node_t *)data;
    (void)param;

    VERIFY(node->height == RecSubTreeHeight(node));
    VERIFY(IsBalanced(node));
    return 0;
}



enum
{
    HEIGHT_UNCORRECT = 1,
    BALANCE_FACTOR_TOO_HIGH,
    LEFT_CHILD_BIG,
    RIGHT_CHILD_SMALL
};



static int BalanceTestingImpl(avl_node_t *node,
                              int (*compare)(const void *d1, const void *d2))
{
    int r_child_height = 0;
    int l_child_height = 0;
    int left_status = 0;
    int right_status = 0;

    if (NULL == node)
    {
        return 0;
    }

    left_status = BalanceTestingImpl(LEFT_SON(node), compare);
    right_status = BalanceTestingImpl(RIGHT_SON(node), compare);

    if (0 != left_status)
    {
        return left_status;
    }
    if (0 != right_status)
    {
        return right_status;
    }

    r_child_height = GetHeight(RIGHT_SON(node));
    l_child_height = GetHeight(LEFT_SON(node));

    if (node->height != MAX(r_child_height, l_child_height) + 1)
    {
        return HEIGHT_UNCORRECT;
    }
    if (GetBalanceFactor(node) < -1 || GetBalanceFactor(node) > 1)
    {
        return BALANCE_FACTOR_TOO_HIGH;
    }
    if (LEFT_SON(node) && compare(LEFT_SON(node)->data, node->data) > 0)
    {
        return LEFT_CHILD_BIG;
    }
    if (RIGHT_SON(node) && compare(RIGHT_SON(node)->data, node->data) < 0)
    {
        return RIGHT_CHILD_SMALL;
    }

    return 0;
}

int BalanceTesting(avl_t *tree)
{
    return BalanceTestingImpl(tree->root, tree->compare);
}


#endif
