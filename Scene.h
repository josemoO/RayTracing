/* 
 * File:   Scene.h
 * Author: jose
 *
 * Created on June 3, 2013, 9:36 PM
 */

#ifndef SCENE_H
#define	SCENE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Lists.h"
#define MAX_LINE_LENGTH 10000

    long double ambientLighting_IA;

    
    int xMax, yMax, xMin, yMin; //These variables control the window size
    int mirrorLevels;           //Deep of the mirrors
    VECTOR eyePosition; //This variable represent the position of the eye
    
    /* If I import a couple of times one header (As this one in the main.c and scene.c files) the
     definitions are duplicated, in order to avoid this issue, you can mark the variables as extern
     so the compiler recognize them but don't it by himself, you can do it in any other c file*/
    extern LIST_NODE_PTR sceneList; //The scene is an array of objects
    extern LIST_NODE_PTR lightsList;

    void readScene(char* fileName);
    void fillOutCorners();
    void getGeneralObjectInfo(char *line, NODE_DATA *currentNode);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* SCENE_H */

