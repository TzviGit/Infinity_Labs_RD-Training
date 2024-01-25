#ifndef __AVL_UTIL_HEADER_TZVI__
#define __AVL_UTIL_HEADER_TZVI__

#include "avl_tree.h"


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





#endif
