/**
 * Implements functions of queue data structure with 2 links
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "treeNode.h"
#include "frontierNode.h"



bool isEmpty(frontier_node *head) {
    if (head == NULL) {
        return true;
    }
    return false;
}

int enqueueStar(frontier_node **head, frontier_node **tail, tree_node *leaf) {
    if (leaf == NULL)return 1;
    frontier_node *node = (frontier_node *) malloc(sizeof(frontier_node));
    if (node == NULL)return 1;
    node->leaf = leaf;

    if (*head == NULL && *tail == NULL) {
        *head = *tail = node;
        return 0;
    }

    if ((node->leaf->starDistance) < (*head)->leaf->starDistance) {
        node->next = *head;
        *head = node;
        return 0;
    }

    frontier_node *current = *head;
    while (current->next != NULL && ((current->next->leaf->starDistance) <= (node->leaf->starDistance))) {
        current = current->next;
    }
    if (current == *tail) {
        node->next = NULL;
        node->previous = current;
        current->next = node;
        *tail = node;
        return 0;
    }
    node->next = current->next;
    current->next->previous = node;
    current->next = node;
    node->previous = current;
    return 0;

}

int enqueueBest(frontier_node **head, frontier_node **tail, tree_node *leaf) {
    if (leaf == NULL)return 1;
    frontier_node *node = (frontier_node *) malloc(sizeof(frontier_node));
    if (node == NULL)return 1;
    node->leaf = leaf;
    if (*head == NULL && *tail == NULL) {
        *head = *tail = node;
        return 0;
    }
    if ((node->leaf->distanceFromGoal) < (*head)->leaf->distanceFromGoal) {
        node->next = *head;
        *head = node;
        return 0;
    }
    frontier_node *current = *head;
    while (current->next != NULL && ((current->next->leaf->distanceFromGoal) <= (node->leaf->distanceFromGoal))) {
        current = current->next;
    }
    if (current == *tail) {
        node->next = NULL;
        node->previous = current;
        current->next = node;
        *tail = node;
        return 0;
    }
    node->next = current->next;
    current->next->previous = node;
    current->next = node;
    node->previous = current;
    return 0;
}

int enqueue(frontier_node **head, frontier_node **tail, tree_node *leaf) {
    if (leaf == NULL)return 1;
    frontier_node *node = (frontier_node *) malloc(sizeof(frontier_node));
    if (node == NULL)return 1;
    node->leaf = leaf;
    node->next = NULL;
    node->previous = *tail;
    if (*head == NULL && *tail == NULL) {
        *head = *tail = node;
        return 0;
    }
    (*tail)->next = node;
    (*tail) = node;
    return 0;
}

tree_node *dequeue(frontier_node **head, frontier_node **tail) {
    if (*head == NULL || *tail == NULL) return NULL;
    frontier_node *temp = *head;
    if (*head == *tail) {
        *head = *tail = NULL;
    }
    ((*head)->next)->previous = NULL;
    *head = (*head)->next;
    free(temp);
    return temp->leaf;
}