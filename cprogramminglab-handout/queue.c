/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q != NULL)
    {
      q->head = NULL;
      q->tail = NULL;
      q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements? */
    if (q != NULL)
    {
      list_ele_t *p1 = q->head;
      list_ele_t *p2;
      while (p1 != NULL)
      {
        p2 = p1->next;
        free(p1);
        p1 = p2;
      }
        
    /* Free queue structure */
      free(q);
    }
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_head(queue_t *q, int v)
{
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    if (q == NULL)
    {
      return false;
    }
    newh = malloc(sizeof(list_ele_t));
    /* What if malloc returned NULL? */
    if (newh == NULL)
    {
      return false;
    }
    newh->value = v;
    newh->next = q->head;
    q->head = newh;
    q->size++;
    //如果链表为空，则尾指针指向插入节点。
    if (q->tail == NULL)
    {
      q->tail = newh;
    }
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_tail(queue_t *q, int v)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
    {
        return false;
    }
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL)
    {
      return false;
    }
    newt->value = v;
    newt->next = NULL;
    if (q->size != 0)
    {
      q->tail->next = newt;
    }
    q->tail = newt;
    q->size++;
    if (q->head == NULL)
    {
      q->head = newt;
    }
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed
*/
bool q_remove_head(queue_t *q, int *vp)
{
    /* You need to fix up this code. */
    if (q == NULL || q->head == NULL)
    {
      return false;
    }
    list_ele_t *oldh = q->head;
    if (vp != NULL)
    {
      *vp = q->head->value;
    }
    if (q->size == 1)
    {
      q->head = NULL;
      q->tail = NULL;
    }
    else
    {
      q->head = q->head->next;
    }
    free(oldh);
    q->size--;
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
    {
      return 0;
    }
    return q->size;
}

/*
  Reverse elements in queue.

  Your implementation must not allocate or free any elements (e.g., by
  calling q_insert_head or q_remove_head).  Instead, it should modify
  the pointers in the existing data structure.
 */
void q_reverse(queue_t *q)
{
    /* You need to write the code for this function */
    if (q == NULL | q->size != 0)
    {
      return;
    }
    list_ele_t *p1 = q->head;
    list_ele_t *p2 = q->head->next;
    list_ele_t *temp;
    while (p2 != NULL)
    {
      temp = p2;
      p2->next = p1;
      p1 = p2;
      p2 = temp->next;
    }
    q->head->next = NULL;
}
