/* 
 * File:   Utils.h
 * Author: jose
 *
 * Created on June 3, 2013, 9:19 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Structures_Definitions.h"

    long double dotProduct(const VECTOR *a,const VECTOR *b);
    long double calculateNormBetweenTwoVectors(VECTOR *a, VECTOR *b);
    long double calculateQuadraticEquationResults(long double *a, long double *b, long double *c, int type);
    VECTOR crossProduct(VECTOR *a, VECTOR *b);
    void getPlaneComponents(POINT point1, POINT point2, POINT point3, VECTOR *planeNormal, long double *componentD);
    void getComponentD(VECTOR *planeNormal, VECTOR *point, long double *componentD);
    long double calculateNormOfVector(VECTOR *a);
    LIST_NODE_PTR translateToOrigin(LIST_NODE_PTR pointsList, long double x, long double y, NODE_DATA *lastPoint);
    void calculatePlaneIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *object, INTERSECTION *intersection, int *wasIntersectionFound);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

