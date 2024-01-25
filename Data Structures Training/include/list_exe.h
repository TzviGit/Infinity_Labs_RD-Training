#ifndef LIST_EXE_H
#define LIST_EXE_H

typedef struct node node_t;
node_t *Flip(node_t *head);

int HasLoop(const node_t *head);

node_t *FindIntersection(node_t *head_1, node_t *head_2);

#endif /*list_exe.h defined*/
