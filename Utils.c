
#include "Utils.h"
#include "Lists.h"
#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <values.h>

long double dotProduct(const VECTOR *a, const VECTOR *b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

long double calculateNormBetweenTwoVectors(VECTOR *a, VECTOR *b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2));
}

/*
 * This method get the value of T, it is calculate using a quadratic equation
 * The type parameters indicates if is the positive(1) or negative(0) case
 */
long double calculateQuadraticEquationResults(long double *a, long double *b, long double *c, int type) {
    long double t;
    if (type == 1) {
        t = ((-1 * *b) + sqrt(pow(*b, 2) - 4 * *a * *c)) / 2;
    } else {
        t = ((-1 * *b) - sqrt(pow(*b, 2) - 4 * *a * *c)) / 2;
    }

    return t;
}

/*
 * The cross product return a Vector who is orthogonal to both Vectors
 */
VECTOR crossProduct(VECTOR *a, VECTOR *b) {
    VECTOR result;
    result.x = a->y * b->z - a->z * b->y;
    result.y = -1 * (a->x * b->z - a->z * b->x);
    result.z = a->x * b->y - a->y * b->x;
    return result;
}

/*
 * The equation of a plane is Ax + By + Cz + D = 0, with 3 points we can get a plane.
 * This method returns the normal of the plane (Who has the components A, B, Z) and the component D
 */
void getPlaneComponents(POINT point1, POINT point2, POINT point3, VECTOR *planeNormal, long double *componentD) {
    VECTOR a, b;
    long double distanceOfNormal;

    //First step: get the two Vector of the plane
    a.x = point2.x - point1.x;
    a.y = point2.y - point1.y;
    a.z = point2.z - point1.z;

    b.x = point3.x - point1.x;
    b.y = point3.y - point1.y;
    b.z = point3.z - point1.z;

    //Second step: get the normal of the plane, who is got using the cross product
    *planeNormal = crossProduct(&a, &b);
    distanceOfNormal = calculateNormOfVector(planeNormal);

    //Third step: get the d component
    *componentD = -((*planeNormal).x * point1.x) -((*planeNormal).y * point1.y) -((*planeNormal).z * point1.z);

    //Finally I normalize the components
    planeNormal->x = planeNormal->x / distanceOfNormal;
    planeNormal->y = planeNormal->y / distanceOfNormal;
    planeNormal->z = planeNormal->z / distanceOfNormal;

    *componentD = *componentD / distanceOfNormal;
}

/*
 * This method already receives the plane normal, so it only get the component D
 */
void getComponentD(VECTOR *planeNormal, VECTOR *point, long double *componentD) {
    long double distanceOfNormal;

    distanceOfNormal = calculateNormOfVector(planeNormal);

    //Third step: get the d component
    *componentD = -(planeNormal->x * point->x) -(planeNormal->y * point->y) -(planeNormal->z * point->z);

    //Finally I normalize the components
    planeNormal->x = planeNormal->x / distanceOfNormal;
    planeNormal->y = planeNormal->y / distanceOfNormal;
    planeNormal->z = planeNormal->z / distanceOfNormal;

    *componentD = *componentD / distanceOfNormal;
}

long double calculateNormOfVector(VECTOR *a) {
    long double result = sqrt(pow(a->x, 2) + pow(a->y, 2) + pow(a->z, 2));
    return result;
}

/*
 * This method translate to the origin (Who is the x,y point) a list of points passed as parameter
 */
LIST_NODE_PTR translateToOrigin(LIST_NODE_PTR pointsList, long double u, long double v, NODE_DATA *lastPoint) {

    LIST_NODE_PTR currentPoint = pointsList, newPointList = NULL;
    NODE_DATA newNode;

    while (currentPoint != NULL) {
        newNode.point.x = u - currentPoint->data.point.x;
        newNode.point.y = v - currentPoint->data.point.y;
        insert(&newPointList, newNode);

        currentPoint = currentPoint->nextPtr;
    }
    *lastPoint = newNode;
    return newPointList;

}

void calculatePlaneIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *object, INTERSECTION *intersection, int *wasIntersectionFound) {
    long double t;
    VECTOR intersectionPoint;
    *wasIntersectionFound = 0;

    //First I check that there's an intersection with the plane, I use the product point for it
    //long double dotProductRayPlane = dotProduct(&object->normal, &rayFromEyeDirection);
    long double dotProductRayPlane = dotProduct(rayFromEyeDirection, &object->dependedNormal);

    if (dotProductRayPlane > EPSILON) {
        t = -((object->dependedNormal.x * eyePosition->x) + (object->dependedNormal.y * eyePosition->y) + (object->dependedNormal.z * eyePosition->z) + object->dependedComponentD) / dotProductRayPlane;

        //Second I get the intersection using the t
        if (t > EPSILON) {//Only if the distance (t) is greater than 0 we say that we found an intersection with the plane
            intersectionPoint.x = eyePosition->x + t * rayFromEyeDirection->x;
            intersectionPoint.y = eyePosition->y + t * rayFromEyeDirection->y;
            intersectionPoint.z = eyePosition->z + t * rayFromEyeDirection->z;

            *wasIntersectionFound = 1;
            intersection->t = t;
            intersection->intersection = intersectionPoint;
            intersection->object = *object;

        }
    }


}