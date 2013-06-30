/* 
 * File:   Polygon.h
 * Author: jose
 *
 * Created on June 10, 2013, 10:27 PM
 */

#ifndef POLYGON_H
#define	POLYGON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Structures_Definitions.h"
#include "Utils.h"
#include "Lists.h"
    
    void polygonIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *object, LIST_NODE_PTR *intersections);
    int isThePointInsideThePolygon(OBJECT *object, VECTOR *intersection);
    LIST_NODE_PTR splashPolygon(LIST_NODE_PTR pointsList, VECTOR *planeNormal);
    void getUV(long double *u, long double *v, VECTOR *planeNormal, VECTOR *interseption);


#ifdef	__cplusplus
}
#endif

#endif	/* POLYGON_H */

