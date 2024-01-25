/********************************************************
 *
 * QUIZ QUESTION --RemoveLoop in singlylinked list ~~
 *
 * Author : Tzvi Moskowitz
 *
 * Date:  October 25, 2021
 *
 * OLRD 110 Ramat Gan
 *
 * *********************************************************/


#include "test_util.h"

typedef struct node
{
    void  *data;
    struct node *next;
}node_t;

#define NEXT_NODE(node) (node)->next

node_t *FindLoopIfExists(node_t *head)
{
    node_t *turtle = head;
    node_t *rabbit = head;

    while (NULL != rabbit && NULL != NEXT_NODE(rabbit))
    {
        rabbit = NEXT_NODE(NEXT_NODE(rabbit));
        turtle = NEXT_NODE(turtle);
    }

    return rabbit;
}

void RemoveLoop(node_t *head)
{
    node_t *walk_ahead = head;
    node_t *circler = FindLoopIfExists(head);
    node_t *place_holder = circler;
    node_t *restart = head;

    if (NULL == place_holder)
    {
        return;
    }

    while (place_holder != NEXT_NODE(circler))
    {
        circler = NEXT_NODE(circler);
        walk_ahead = NEXT_NODE(walk_ahead);
    }


    while (walk_ahead->next != restart->next)
    {
        restart = NEXT_NODE(restart);
        walk_ahead = NEXT_NODE(walk_ahead);
    }

    NEXT_NODE(walk_ahead) = NULL;
}
