/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Version       1.1.1                                           *
 ~ Author        OL110 Students                                  ~
 * Reviewer      Binyamin                                        *
 ~ Description   AVL Tree - Header Module                        ~
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __AVL_OL_110_H__
#define __AVL_OL_110_H__

#include <stddef.h> /* size_t */

typedef enum
{
    PRE,
    IN,
    POST
} order_t;

typedef struct avl avl_t;

/* Input: compare function - returns a positive integer if 'd1'
   should be after 'd2', 0 if the order is the same, and a negative
   numberif 'd1' should be before 'd2'.
   Return: avl_t pointer on success, NULL on failure. */
avl_t *AVLCreate(int (*compare)(const void *d1, const void *d2));

/* Input: avl_t poniter.
   Return: - */
void AVLDestroy(avl_t *avl);

/* Input: avl_t poniter,
          data to be removed.
   Return: removed data or NULL if not found. */
void *AVLRemove(avl_t *avl, const void *to_remove);

/* creates and inserts node with the given data. */
/* Input: avl_t poniter,
          data to be inserted.
   Return: 0 on success, non-zero on failure. */
int AVLInsert(avl_t *avl, const void *data);

/* Input: avl_t poniter */
/* Return: number of nodes in the tree. */
size_t AVLSize(const avl_t *avl);

/* Input: avl_t poniter */
/* Return: tree height (number of levels). */
/* Height of tree with single node is 0 */
/* Height of empty tree might be -1 - implementation defined */
int AVLHeight(const avl_t *avl);

/* Input: avl_t poniter */
/* Return: non-zero if empty, 0 otherwise. */
int AVLIsEmpty(const avl_t *avl);

/* Input: avl_t poniter, data to be find */
/* Return : found data or NULL if not found */
void *AVLFind(const avl_t *avl, const void *to_find);

/* invokes the action function on all nodes.
   Input: avl_t pointer,
          action function,
          param - to be passed as an argument to action function,
          order_t - traversing method.
   Return: 0 on success or action_failure_status otherwise,
            halts execution on first failure of action. */
int AVLForEach(avl_t *avl, int (*action)(void *data, void *param),
               void *param, order_t order);



#endif /* __AVL_OL_110_H__ */
