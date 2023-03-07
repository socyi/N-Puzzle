#ifndef HW3_TREENODE_H
#define HW3_TREENODE_H

#include <stdio.h>
#include <stdbool.h>

//Tree with 1 to 4 childs data structure
typedef struct tree_node
{
    int **puzzle;
    int distanceFromGoal;
    int starDistance;
    struct tree_node *parent;
    struct tree_node *right;
    struct tree_node *down;
    struct tree_node *left;
    struct tree_node *up;
} tree_node;

/**Creates a new tree node and inserts the node in the tree
 *
 * @param array array to be stored in the tree node
 * @param parent parent node
 * @param solution solution of puzzle
 * @return the newly inserted tree node
 */
tree_node *insertTreeNode(int **array, tree_node *parent, int **solution);

/**Finds the number of steps from the final node to the root
 *
 * @param node
 * @return number of steps
 */
int findPath(tree_node *node) ;

/**Deletes the tree and frees it from the heap memory
 *
 * @param node
 */
void deleteTree(tree_node* node);

/**Finds the path(moves) from the root until the solution
 *
 * @param node
 * @param temp
 * @param string String where the path is stored
 */
void findPathStr(tree_node *node, tree_node *temp,char **string) ;

/**Checks if the array of the node is the same with the array of its ancestors
 *
 * @param node
 * @param puzzle
 * @return  true or false
 */
bool childToRoot(tree_node *node, int **puzzle) ;

#endif //HW3_TREENODE_H
