/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Version       2.0.0                                           *
 ~ Author        OL110 Students                                  ~
 * Reviewer      Yinon Yishay                                    *
 ~ Description   Binary Search Tree - Source_File                ~
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __BST_OL_110_H__
#define __BST_OL_110_H__

#include <stddef.h> /* size_t */

typedef struct bst bst_t;
typedef struct bst_node *bst_iter_t;

typedef int (*find_func)(const void *, const void *, void *param);
typedef int (*action_func)(void *data, void *param);


/* O(1) */
/* returns a pointer to bst, on success, or NULL on failure */
bst_t *BSTCreate(find_func is_before, void *param);

/* O(n) */
/* input: tree pointer */
/* destroys associated tree */
void BSTDestroy(bst_t *bst);

/* O(n) */
/* input: tree pointer */
/* return: num of nodes in the tree */
size_t BSTSize(const bst_t *bst);

/* O(1) */
/* input: tree pointer */
/* return:                    */
/*      tree is empty -> 1    */
/*      tree isn't empty -> 0 */
int BSTIsEmpty(const bst_t *bst);

/* O(log n) */
/* average complexity (assume independent and identically distributed data) */
/* returns an iterator to the new node holding 'data', on success,
	or BAD_ITER on failure */
/* input:                       */
/*      tree pointer            */
/*      data                    */
/* creates a node with given data */
/* inserts node in appropriate place */
/* returns an iterator to the node */
/* returns end in failure */
bst_iter_t BSTInsert(bst_t *bst, void *data);

/* O(log n) */
/* average complexity (assume independent and identically distributed data) */
/* removes the node iter is pointing to,
	and returns removed iter's data */
void *BSTRemove(bst_iter_t iter);

/* O(log n) */
/* find the smallest member of the tree */
bst_iter_t BSTBegin(const bst_t *bst);

/* O(1) */
/* returns end (invalid) */
bst_iter_t BSTEnd(const bst_t *bst);

/* O(log n) */
/* returns the previous node according to the hierarchy of the tree */
bst_iter_t BSTPrev(bst_iter_t iter);

/* O(log n) */
/* returns the next node according to the hierarchy of the tree */
bst_iter_t BSTNext(bst_iter_t iter);

/* O(1) */
int BSTIterIsEqual(bst_iter_t iter1, bst_iter_t iter2);

/* O(1) */
void *BSTGetData(bst_iter_t iter);

/* O(log n) */
bst_iter_t BSTFind(const bst_t *bst, void *data);

/* O(n) */
int BSTForEach(bst_iter_t from, bst_iter_t to,
			   action_func action, void *param);

#endif /* __BST_OL_110_H__ */
