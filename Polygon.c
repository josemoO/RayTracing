#include "Polygon.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void polygonIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *object, LIST_NODE_PTR *intersections) {

    int wasIntersectionWithPlaneFound;
    INTERSECTION intersectionWithPlane, polygonIntersection;

    if (dotProduct(rayFromEyeDirection, &object->normal) < EPSILON) {
        VECTOR newN;
        newN.x = -1 * object->normal.x;
        newN.y = -1 * object->normal.y;
        newN.z = -1 * object->normal.z;
        object->dependedNormal = newN;
        object->dependedComponentD = -1 * object->componentD;
    }else{
        object->dependedNormal = object->normal;
        object->dependedComponentD = object->componentD;
    }

    calculatePlaneIntersection(eyePosition, rayFromEyeDirection, object, &intersectionWithPlane, &wasIntersectionWithPlaneFound);

    if (wasIntersectionWithPlaneFound == 1) {
        //Now we have to return an intersection only if the intersection is INSIDE THE POLYGON (Not just the plane)
        if (isThePointInsideThePolygon(object, &intersectionWithPlane.intersection) == 1) {
            polygonIntersection.t = intersectionWithPlane.t;
            polygonIntersection.intersection = intersectionWithPlane.intersection;

            polygonIntersection.object = *object;
            insertInOrder(intersections, polygonIntersection);
        }
    }

}

/*
 * This method evaluates if the intersection is inside the polygon or not, in order to to this, it does a parallel projection of the polygon
 * Then it translate the polygon to the origin, and finally using the x axe, if the number of edges that the x axe touch is odd the intersection is inside the polygon
 * if is is pair, the intersection is not in the polygon
 */
int isThePointInsideThePolygon(OBJECT *object, VECTOR *intersection) {
    LIST_NODE_PTR pointList = ((LIST_NODE_PTR) object->polygon.listOfPointsSplashPtr);
    LIST_NODE_PTR startNode = pointList;
    int numberOfWalls = 0;
    long double u, v;
    int isTheFirstOne = 1, checkLastOne = 0;
    POINT currentPoint, nextPoint, previousPoint, lastPoint;

    //First I have to get u, v values which were used to splash the polygon (Made on the load fase)
    getUV(&u, &v, &object->normal, intersection);

    //Second I have to translate the polygon to the origin in order to "Count the walls"
    while (pointList != NULL) {//I walk the list until the penultimate element, this is because I evaluate the edges and not the points
        currentPoint.x = u - pointList->data.point.x;
        currentPoint.y = v - pointList->data.point.y;

        if (pointList->nextPtr != NULL) { //Case it is not the last point
            nextPoint.x = u - pointList->nextPtr->data.point.x;
            nextPoint.y = v - pointList->nextPtr->data.point.y;
        } else { //Case it is the last point
            nextPoint.x = u - startNode->data.point.x;
            nextPoint.y = v - startNode->data.point.y;

            if (checkLastOne == 1) {
                long double nextX, nextY, previousX, previousY;
                previousX = currentPoint.x;
                previousY = currentPoint.y;
                nextX = startNode->nextPtr->data.point.x;
                nextY = startNode->nextPtr->data.point.y;

                if ((previousY > 0.0 && nextY > 0.0) || (previousY < 0.0 && nextY < 0.0)) {
                    numberOfWalls += 2;
                } else {
                    numberOfWalls++;
                }
            }

        }

        //Logic of the corners, If a corner is a minum or a maximun we have to count two if it is a nee we count it only as one
        if (currentPoint.y == 0.0 && currentPoint.x > 0.0) {
            long double nextX, nextY, previousX, previousY;
            if (isTheFirstOne != 1) {//The previous X and Y is in the last node of the list
                previousX = previousPoint.x;
                previousY = previousPoint.y;

                nextX = nextPoint.x;
                nextY = nextPoint.y;

                if ((previousY > 0.0 && nextY > 0.0) || (previousY < 0.0 && nextY < 0.0)) {
                    numberOfWalls += 2;
                } else {
                    numberOfWalls++;
                }
            } else {
                checkLastOne = 1; //Case where I have to check the first corner
            }
        }

        //trivial acceptance. Both U are positive and one v is negative and the another is positive
        if ((currentPoint.x >= 0.0 && nextPoint.x >= 0.0) &&
                ((currentPoint.y > 0.0 && nextPoint.y < 0.0) ||
                (currentPoint.y < 0.0 && nextPoint.y > 0.0))) {
            numberOfWalls++;
        } else if (((currentPoint.x >= 0.0 && nextPoint.x <= 0.0) ||
                (currentPoint.x <= 0.0 && nextPoint.x >= 0.0)) &&
                ((currentPoint.y > 0.0 && nextPoint.y < 0.0) ||
                (currentPoint.y < 0.0 && nextPoint.y > 0.0))) {//Hard Case, One v positive and the another one negative, one u positive and the another one negative
            long double m = (nextPoint.y - currentPoint.y) / (nextPoint.x - currentPoint.x);
            long double b = currentPoint.y - (m * currentPoint.x);
            long double xIntersection = (0 - b) / m;

            if (xIntersection > 0.0) {
                numberOfWalls++;
            }
        }

        previousPoint = currentPoint;

        pointList = pointList->nextPtr;
        isTheFirstOne = 0;
    }

    return (numberOfWalls % 2);
}

/*
 * In order to now if the intersection of the plane is inside the polygon or not, we have to "Count Walls"
 * Count the edges in 3D is difficult so we do a parallel projection, this is take one of the axes and delete it.
 * Which axe should we delete? The one who has the greater area, this is the one with the greatest absolute value on the plane equation
 */
LIST_NODE_PTR splashPolygon(LIST_NODE_PTR pointsList, VECTOR *planeNormal) {
    LIST_NODE_PTR currentPoint = pointsList;
    LIST_NODE_PTR newPointsList = NULL;
    NODE_DATA newNode;
    //First step: We have to splash the plane to transform the problem to 2D, we do this by deleting and Axe, the one who component has the greater absolute value
    if (fabs(planeNormal->x) >= fabs(planeNormal->y) && fabs(planeNormal->x) >= fabs(planeNormal->z)) { //The x has the greatest area

        while (currentPoint != NULL) {
            newNode.point.x = currentPoint->data.point.y;
            newNode.point.y = currentPoint->data.point.z;

            insert(&newPointsList, newNode);
            currentPoint = currentPoint->nextPtr;
        }
    } else if (fabs(planeNormal->y) >= fabs(planeNormal->x) && fabs(planeNormal->y) >= fabs(planeNormal->z)) { //The y has the greatest area

        while (currentPoint != NULL) {
            newNode.point.x = currentPoint->data.point.x;
            newNode.point.y = currentPoint->data.point.z;

            insert(&newPointsList, newNode);
            currentPoint = currentPoint->nextPtr;
        }
    } else { //The z has the greatest area
        newPointsList = currentPoint;
    }

    return newPointsList;
}

void getUV(long double *u, long double *v, VECTOR *planeNormal, VECTOR *interseption) {
    if (fabs(planeNormal->x) >= fabs(planeNormal->y) && fabs(planeNormal->x) >= fabs(planeNormal->z)) { //The x has the greatest area
        *u = interseption->y;
        *v = interseption->z;

    } else if (fabs(planeNormal->y) >= fabs(planeNormal->x) && fabs(planeNormal->y) >= fabs(planeNormal->z)) { //The y has the greatest area
        *u = interseption->x;
        *v = interseption->z;
    } else { //The z has the greatest area
        *u = interseption->x;
        *v = interseption->y;
    }
}
