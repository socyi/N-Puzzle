/** @file main.c
* @brief Automatic N puzzle solver
*
* Solves a given N sized puzzle using 1 of the 3 implemented algorithms: Breadth-first search
* Best-first or A-star. Prints the solution path on a text file.
*
* @author Socrates Yiannakou
* @bug Writing on a file is inconsistent. Sometimes the output file is not created and the program
 * does not terminate normally. To avoid this, I have the output txt file created before compiling and
 * running this program. Also a character (A) appears near the beginning of the output file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "treeNode.h"
#include "frontierNode.h"

/**
 * While the queue is not empty, the head of the queue is removed and is available for processing.
 * Then a new tree node is created, for each possible movement of 0 in the puzzle. According to
 * which algorithm the user has selected, a different enqueue method is used. If the node which
 * got dequeued has the final puzzle, the path from the root that node is written on a text file.
 *
 * @param root root of the tree
 * @param solution solution of the puzzle
 * @param head head of queue
 * @param tail tail of queue
 * @return
 */
int solvePuzzle(tree_node *root, int **solution, frontier_node *head, frontier_node *tail);

/**Checks if 2 arrays-puzzles are equal
 *
 *
 * @param array
 * @param solution
 * @return true or false
 */
bool areEqual(int **array, int **solution);

/** Copies 1 array to another array
 *
 * @param source array
 * @param destination array
 */
void copyArray(int **source, int **destination);

/**Finds the 0-blank square of the array
 *
 * @param array
 * @param iZ stores the row of 0
 * @param jZ stores the column of 0
 */
void findZero(int **array, int *iZ, int *jZ);

/**Calculates the Manhattan distance between 2 arrays-puzzles
 *
 * @param source first array
 * @param destination second array
 * @return integer total distance
 */
int calculateDistance(int **source, int **destination);

int size = 3;
char fileName[50];//4rth argument, name of the output txt file
char algorithm[50]; //2nd argument, 1 of the 3 running choices
int **rootPuzzle; //original puzzle

int calculateDistance(int **source, int **destination) {
    int sum = 0;
    int i, j, m, n;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (source[i][j] != 0) {
                for (m = 0; m < size; m++) {
                    for (n = 0; n < size; n++) {
                        if (source[i][j] == destination[m][n]) {
                            sum += abs(i - m);
                            sum += abs(j - n);
                        }
                    }
                }
            }
        }
    }
    return sum;
}

void findZero(int **array, int *iZ, int *jZ) {
    int j, i;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (array[i][j] == 0) {
                *iZ = i;
                *jZ = j;
                break;
            }
        }
    }
}


void copyArray(int **source, int **destination) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            destination[i][j] = source[i][j];
        }
    }
}

bool areEqual(int **array, int **solution) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (array[i][j] != solution[i][j]) {
                return false;
            }
        }
    }
    return true;
}

int findPath(tree_node *node) {
    if (node == NULL || node->parent == NULL) return 0;
    return findPath(node->parent) + 1;

}

void deleteTree(tree_node *node) {
    int i;
    if (node == NULL) return;
    deleteTree(node->left);
    deleteTree(node->right);
    deleteTree(node->down);
    deleteTree(node->up);

    for (i = 0; i < size; i++) {
        free(node->puzzle[i]);
    }
    free(node->puzzle);
    free(node);
}

void findPathStr(tree_node *node, tree_node *temp, char **string) {
    if (node->parent == NULL) return;
    if (node->parent->right != NULL && areEqual(node->puzzle, ((node->parent)->right)->puzzle)) {
        temp = node;
        findPathStr(node->parent, temp, string);
        strcat(*string, "right\n");
    } else if (node->parent->left != NULL && areEqual(node->puzzle, ((node->parent)->left)->puzzle)) {
        temp = node;
        findPathStr(node->parent, temp, string);
        strcat(*string, "left\n");
    } else if (node->parent->down != NULL && areEqual(node->puzzle, ((node->parent)->down)->puzzle)) {
        temp = node;
        findPathStr(node->parent, temp, string);
        strcat(*string, "down\n");
    } else if (node->parent->up != NULL && areEqual(node->puzzle, ((node->parent)->up)->puzzle)) {
        temp = node;
        findPathStr(node->parent, temp, string);
        strcat(*string, "up\n");
    }
}


bool childToRoot(tree_node *node, int **puzzle) {
    if (node->parent == NULL) return true;
    while (node->parent != NULL) {
        if (areEqual((node->parent)->puzzle, puzzle)) {
            return false;
        }
        node = node->parent;
    }
    return true;
}

tree_node *insertTreeNode(int **array, tree_node *parent, int **solution) {
    int i;
    tree_node *node = (tree_node *) malloc(sizeof(tree_node));
    if (node == NULL) {
        return NULL;
    }
    int **puz = (int **) malloc(sizeof(int *) * size);

    for (i = 0; i < size; i++) {
        puz[i] = (int *) malloc(sizeof(int) * size);
    }
    copyArray(array, puz);
    node->puzzle = puz;
    node->left = NULL;
    node->right = NULL;
    node->up = NULL;
    node->down = NULL;
    node->parent = parent;

    node->distanceFromGoal = calculateDistance(node->puzzle, solution);
    node->starDistance = node->distanceFromGoal + calculateDistance(node->puzzle, rootPuzzle);
    return node;
}

int solvePuzzle(tree_node *root, int **solution, frontier_node *head, frontier_node *tail) {

    bool solved = false;
    int iZero, jZero, i, j;
    enqueue(&head, &tail, root);

    while (!isEmpty(head)) {
        tree_node *temp = dequeue(&head, &tail);

        if (areEqual(temp->puzzle, solution)) {
            solved = true;
            printf("\n\nSolved\n\n");
            for (i = 0; i < size; i++) {
                for (j = 0; j < size; j++) {
                    printf("%d ", temp->puzzle[i][j]);
                }
                printf("\n");
            }
            FILE *f = NULL;
            f = fopen(fileName, "w");
            if (f == NULL)return 1;
            int count = findPath(temp);
            fprintf(f, "%d\n", count);
            tree_node *temp2 = NULL;
            char *string = (char *) malloc(sizeof(char) * 1000);
            findPathStr(temp, temp2, &string);
            fprintf(f, "%s", string);
            free(string);
            fclose(f);
            break;

        } else {
            findZero(temp->puzzle, &iZero, &jZero);

            if ((jZero + 1) < size) {

                int **temparray = (int **) malloc(sizeof(int *) * size);
                for (i = 0; i < size; i++) {
                    temparray[i] = (int *) malloc(sizeof(int) * size);
                }
                copyArray(temp->puzzle, temparray);
                int tempValue = temparray[iZero][jZero];
                temparray[iZero][jZero] = temparray[iZero][jZero + 1];
                temparray[iZero][jZero + 1] = tempValue;

                if (childToRoot(temp, temparray)) {

                    tree_node *node = insertTreeNode(temparray, temp, solution);
                    temp->right = node;

                    if (strcmp(algorithm, "best") == 0) {
                        enqueueBest(&head, &tail, node);
                    } else if (strcmp(algorithm, "a-star") == 0) {
                        enqueueStar(&head, &tail, node);
                    } else {
                        enqueue(&head, &tail, node);
                    }
                    for (i = 0; i < size; i++)
                        free(temparray[i]);
                    free(temparray);
                }
            }
            if ((iZero + 1) < size) {

                int **temparray = (int **) malloc(sizeof(int *) * size);
                for (i = 0; i < size; i++) {
                    temparray[i] = (int *) malloc(sizeof(int) * size);
                }
                copyArray(temp->puzzle, temparray);
                int tempValue = temparray[iZero][jZero];
                temparray[iZero][jZero] = temparray[iZero + 1][jZero];
                temparray[iZero + 1][jZero] = tempValue;

                if (childToRoot(temp, temparray)) {

                    tree_node *node = insertTreeNode(temparray, temp, solution);
                    temp->down = node;

                    if (strcmp(algorithm, "best") == 0) {
                        enqueueBest(&head, &tail, node);
                    } else if (strcmp(algorithm, "a-star") == 0) {
                        enqueueStar(&head, &tail, node);
                    } else {
                        enqueue(&head, &tail, node);
                    }
                    for (i = 0; i < size; i++)
                        free(temparray[i]);
                    free(temparray);
                }
            }
            if ((jZero - 1) >= 0) {

                int **temparray = (int **) malloc(sizeof(int *) * size);
                for (i = 0; i < size; i++) {
                    temparray[i] = (int *) malloc(sizeof(int) * size);
                }
                copyArray(temp->puzzle, temparray);
                int tempValue = temparray[iZero][jZero];
                temparray[iZero][jZero] = temparray[iZero][jZero - 1];
                temparray[iZero][jZero - 1] = tempValue;

                if (childToRoot(temp, temparray)) {
                    tree_node *node = insertTreeNode(temparray, temp, solution);
                    temp->left = node;

                    if (strcmp(algorithm, "best") == 0) {
                        enqueueBest(&head, &tail, node);
                    } else if (strcmp(algorithm, "a-star") == 0) {
                        enqueueStar(&head, &tail, node);
                    } else {
                        enqueue(&head, &tail, node);
                    }
                    for (i = 0; i < size; i++)
                        free(temparray[i]);
                    free(temparray);
                }
            }
            if ((iZero - 1) >= 0) {

                int **temparray = (int **) malloc(sizeof(int *) * size);
                for (i = 0; i < size; i++) {
                    temparray[i] = (int *) malloc(sizeof(int) * size);
                }
                copyArray(temp->puzzle, temparray);
                int tempValue = temparray[iZero][jZero];
                temparray[iZero][jZero] = temparray[iZero - 1][jZero];
                temparray[iZero - 1][jZero] = tempValue;

                if (childToRoot(temp, temparray)) {
                    tree_node *node = insertTreeNode(temparray, temp, solution);
                    temp->up = node;

                    if (strcmp(algorithm, "best") == 0) {
                        enqueueBest(&head, &tail, node);
                    } else if (strcmp(algorithm, "a-star") == 0) {
                        enqueueStar(&head, &tail, node);
                    } else {
                        enqueue(&head, &tail, node);
                    }
                    for (i = 0; i < size; i++)
                        free(temparray[i]);
                    free(temparray);
                }
            }
        }
    }
    if (solved) {
        return 0;

    } else {
        printf("\n\nPuzzle has no solution.");
        return 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("\nWrong Input!\nFormat has to be: executable file_algorithm type_input txt_output txt\n ");
        return 1;
    }

    char s1[] = "breadth";
    char s2[] = "a-star";
    char s3[] = "best";
    char line0[100];
    char *point = NULL;
    char *point2 = NULL;

    int count = 0;
    int x = -1;
    char lineTemp[100];
    int j = 0;
    int i = 0;
    int solutionCount = 1;

    //Checks for input errors
    if (strcmp(s1, argv[1]) != 0 && strcmp(s2, argv[1]) != 0 && strcmp(s3, argv[1]) != 0) {
        printf("\nWrong Input!\nAlgorithm type does not exist.\n ");
        return 1;
    }

    if (strlen(argv[2]) <= 1 || strlen(argv[3]) <= 1) {
        printf("\nWrong Input!\n Text files have wrong format.\n ");
        return 1;
    }
    point = strchr(argv[2], '.');
    point2 = strchr(argv[3], '.');

    if (strcmpi(point, ".txt") != 0) {
        printf("\nWrong Input!\nInput file format is wrong.\n ");
        return 1;
    }

    if (strcmpi(point2, ".txt") != 0) {
        printf("\nWrong Input!\nOutput file format is wrong.\n ");
        return 1;
    }

    FILE *fp = NULL;
    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        printf("\nCould not open file.");
        return 1;
    }

    fgets(lineTemp, 100, fp);
    char *pointTemp = lineTemp;
    strtok(pointTemp, "\n");

    while (*pointTemp) {
        x = strtol(pointTemp, &pointTemp, 10);
        if (x != -1) {
            count++;
            x = -1;
        }
        pointTemp++;
    }
    size = count;
    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        printf("\nCould not open file.");
        return 1;
    }
    strcpy(fileName, argv[3]);
    //Initialises data structures, input puzzle and solution of the puzzle
    int **array = (int **) malloc(sizeof(int *) * size);
    if (array == NULL) return 1;

    for (i = 0; i < size; i++) {
        array[i] = (int *) malloc(sizeof(int) * size);
        if (*array == NULL)return 1;
    }
    int **solution = (int **) malloc(sizeof(int *) * size);
    if (solution == NULL) return 1;

    for (i = 0; i < size; i++) {
        solution[i] = (int *) malloc(sizeof(int) * size);
        if (*solution == NULL)return 1;
    }
    i = 0;

    while (fgets(line0, 100, fp)) {
        char *point3 = line0;
        strtok(point3, "\n");

        while (*point3) {
            x = strtol(point3, &point3, 10);
            if (x != -1) {
                array[i][j] = x;
                solution[i][j] = solutionCount;
                x = -1;
                j++;
                solutionCount++;
            }
            point3++;
        }
        i++;
        j = 0;
    }
    fclose(fp);
    solution[size - 1][size - 1] = 0;

    tree_node *search_tree = (tree_node *) malloc(sizeof(tree_node));
    if (search_tree == NULL)return 1;
    search_tree->parent = NULL;
    search_tree->down = NULL;
    search_tree->up = NULL;
    search_tree->right = NULL;
    search_tree->left = NULL;
    memmove(&search_tree->puzzle, &array, sizeof(search_tree->puzzle));

    frontier_node *frontier_head = (frontier_node *) malloc(sizeof(frontier_node));
    if (frontier_head == NULL)return 1;
    frontier_node *frontier_tail = (frontier_node *) malloc(sizeof(frontier_node));
    if (frontier_tail == NULL)return 1;

    frontier_head = frontier_tail;
    frontier_head->next = NULL;
    frontier_head->previous = NULL;
    frontier_head->leaf = search_tree;
    strcpy(algorithm, argv[1]);
    search_tree->distanceFromGoal = calculateDistance(search_tree->puzzle, solution);
    rootPuzzle = search_tree->puzzle;

    solvePuzzle(search_tree, solution, frontier_head, frontier_tail);

    //frees memory
    for (i = 0; i < size; i++) {
        free(array[i]);
        free(solution[i]);
    }
    free(array);
    free(solution);
    deleteTree(search_tree);
    free(frontier_head);
    free(frontier_tail);
    return 0;
}
