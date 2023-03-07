#ifndef HW3_FRONTIERNODE_H
#define HW3_FRONTIERNODE_H

#include "treeNode.h"
#include <stdbool.h>

//Doubly linked queue structure with tree nodes as elements
typedef struct frontier_node
{
    tree_node *leaf;
    struct frontier_node *next;
    struct frontier_node *previous;
} frontier_node;

/**Checks if the queue is empty
 *
 * @param head head of queue
 * @return true or false
 */
bool isEmpty(frontier_node *head);

/**Inserts an element in a sorted queue based on A* algorithm (based on distance from root+distance from goal)
 *
 * @param head head of queue
 * @param tail tail of queue
 * @param leaf tree node
 * @return
 */
int enqueueStar(frontier_node **head, frontier_node **tail, tree_node *leaf);

/**Inserts an element in a sorted queue based on Best algorithm (based on distance from goal)
 *
 * @param head of queue
 * @param tail of queue
 * @param leaf tree node
 * @return
 */
int enqueueBest(frontier_node **head, frontier_node **tail, tree_node *leaf);

/**Inserts an element at the end-tail of queue
 *
 * @param head head of queue
 * @param tail tail of queue
 * @param leaf tree node
 * @return
 */
int enqueue(frontier_node **head, frontier_node **tail, tree_node *leaf);

/**Removes the first element -Head of the queue. Frees the memory and selects a new head if possible
 *
 * @param head head of queue
 * @param tail tail of queue
 * @return
 */
tree_node *dequeue(frontier_node **head, frontier_node **tail) ;
#endif //HW3_FRONTIERNODE_H
