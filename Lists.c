/* 
 * File:   main.c
 * Author: jose
 *
 * Created on May 27, 2013, 1:55 PM
 */

#include "Lists.h"
#include <stdio.h>
#include <stdlib.h>

void insert(LIST_NODE_PTR *sPtr, NODE_DATA data) {
    LIST_NODE_PTR newNodePtr, currentNodePtr, previousNodePtr;

    newNodePtr = malloc(sizeof (LIST_NODE)); //I assign to the new node the memory that it needs. Remember malloc() returns NULL if the computer does not have enough memory

    if (newNodePtr != NULL) {//The node has valid memory
        newNodePtr->data = data;
        newNodePtr->nextPtr = NULL;

        previousNodePtr = NULL;
        currentNodePtr = *sPtr;

        while (currentNodePtr != NULL) {//Move the pointer to the last position
            previousNodePtr = currentNodePtr;
            currentNodePtr = currentNodePtr->nextPtr;
        }

        if (previousNodePtr == NULL) { //Case the list is empty
            *sPtr = newNodePtr; //Initialize the list with the new Node
        } else {
            previousNodePtr->nextPtr = newNodePtr; //The next element of the previous node is not the new node
        }
    } else {
        printf("The object could not be added to the list since the computer does not have enough memory");
    }
}

void insertInOrder(LIST_NODE_PTR *sPtr, INTERSECTION intersection) {
    LIST_NODE_PTR newNodePtr, currentNodePtr, previousNodePtr;
    NODE_DATA data;
    data.intersection = intersection;

    newNodePtr = malloc(sizeof (LIST_NODE)); //I assign to the new node the memory that it needs. Remember malloc() returns NULL if the computer does not have enough memory

    if (newNodePtr != NULL) {//The node has valid memory
        newNodePtr->data = data;
        newNodePtr->nextPtr = NULL;

        previousNodePtr = NULL;
        currentNodePtr = *sPtr;

        while (currentNodePtr != NULL && currentNodePtr->data.intersection.t < data.intersection.t) {
            previousNodePtr = currentNodePtr;
            currentNodePtr = currentNodePtr->nextPtr;
        }

        if (previousNodePtr == NULL) { 
            newNodePtr->nextPtr = currentNodePtr;
            *sPtr = newNodePtr; 
        } else {
            previousNodePtr->nextPtr = newNodePtr; //The next element of the previous node is not the new node
            newNodePtr->nextPtr = currentNodePtr;
        }
    } else {
        printf("The object could not be added to the list since the computer does not have enough memory");
    }
}

void cleanList(LIST_NODE_PTR *sPtr) {
    LIST_NODE_PTR currentNode = *sPtr, tempPtr;
    while (currentNode != NULL) {
        tempPtr = currentNode;
        currentNode = currentNode->nextPtr;
        free(tempPtr);
    }
}
