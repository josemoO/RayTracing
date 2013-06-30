/* 
 * File:   Lists.h
 * Author: jose
 *
 * Created on May 29, 2013, 11:09 PM
 */

#ifndef LISTS_H
#define	LISTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Structures_Definitions.h"

    


    void insert(LIST_NODE_PTR *sPtr, NODE_DATA data);
    void insertInOrder(LIST_NODE_PTR *sPtr, INTERSECTION intersection);
    void cleanList(LIST_NODE_PTR *sPtr);

#ifdef	__cplusplus
}
#endif

#endif	/* LISTS_H */

