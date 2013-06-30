#include "Utils.h"
#include "Sphere.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * This method returns the intersection with the sphere if it exist, if an intersection is found the reference paramenter *wasIntersectionFound 
 * is returned with the value of 1, if not intersection was found the the parameter *wasIntersectionFound is returned with the value of 0.
 * The parameter *intersection contains the Point were the ray touch the sphere and it also calculates the norm of the sphere.
 */
void sphereIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *sphere, LIST_NODE_PTR *intersections) {
    INTERSECTION sphereIntersectionOne, sphereIntersectionTwo;
    long double a, b, c, d; //Represent the parts of the equation

    //a = pow(rayFromEyeDirection.x, 2) + pow(rayFromEyeDirection.y, 2) + pow(rayFromEyeDirection.z, 2);
    a = 1; //ONLY FOR SPHERES
    b = 2 * ((rayFromEyeDirection->x * (eyePosition->x - sphere->sphere.position.x)) +
            (rayFromEyeDirection->y * (eyePosition->y - sphere->sphere.position.y)) +
            (rayFromEyeDirection->z * (eyePosition->z - sphere->sphere.position.z)));
    c = ((pow(eyePosition->x - sphere->sphere.position.x, 2)) +
            (pow(eyePosition->y - sphere->sphere.position.y, 2)) +
            (pow(eyePosition->z - sphere->sphere.position.z, 2)) -
            pow(sphere->sphere.radius, 2));

    d = pow(b, 2) - (4 * a * c);

    if (d > EPSILON) { //Case the ray touch the sphere in two points

        long double t1 = calculateQuadraticEquationResults(&a, &b, &c, 1);
        long double t2 = calculateQuadraticEquationResults(&a, &b, &c, 0);

        /*
         
        if (EPSILON < t1 && t1 < t2) {
            sphereIntersection.t = t1;
            wasIntersectionFound = 1;
        } else if (EPSILON < t2 && t2 < t1) {
            sphereIntersection.t = t2;
            wasIntersectionFound = 1;
        }
         * */
        if (t1 > EPSILON) {
            sphereIntersectionOne.t = t1;
            sphereIntersectionOne.object = *sphere;

            //I have to calculate the point where the intersection was found
            sphereIntersectionOne.intersection.x = eyePosition->x + (sphereIntersectionOne.t * rayFromEyeDirection->x);
            sphereIntersectionOne.intersection.y = eyePosition->y + (sphereIntersectionOne.t * rayFromEyeDirection->y);
            sphereIntersectionOne.intersection.z = eyePosition->z + (sphereIntersectionOne.t * rayFromEyeDirection->z);

            //I also calculate the normal of the sphere
            sphereIntersectionOne.object.normal = calculateSphereNorm(sphereIntersectionOne.intersection, &sphere->sphere);
            sphereIntersectionOne.object.dependedNormal = sphereIntersectionOne.object.normal;

            insertInOrder(intersections, sphereIntersectionOne);
        }
        
        if (t2 > EPSILON) {
            sphereIntersectionTwo.t = t2;
            sphereIntersectionTwo.object = *sphere;

            //I have to calculate the point where the intersection was found
            sphereIntersectionTwo.intersection.x = eyePosition->x + (sphereIntersectionTwo.t * rayFromEyeDirection->x);
            sphereIntersectionTwo.intersection.y = eyePosition->y + (sphereIntersectionTwo.t * rayFromEyeDirection->y);
            sphereIntersectionTwo.intersection.z = eyePosition->z + (sphereIntersectionTwo.t * rayFromEyeDirection->z);

            //I also calculate the normal of the sphere
            sphereIntersectionTwo.object.normal = calculateSphereNorm(sphereIntersectionTwo.intersection, &sphere->sphere);
            sphereIntersectionTwo.object.dependedNormal = sphereIntersectionTwo.object.normal;

            insertInOrder(intersections, sphereIntersectionTwo);
        }
    }


}

/*
 * This method return a Vector who is perpendicular to the surface of the object
 */
VECTOR calculateSphereNorm(VECTOR intersection, OBJECT_SPHERE *sphere) {
    VECTOR n;
    n.x = (intersection.x - sphere->position.x) / sphere->radius;
    n.y = (intersection.y - sphere->position.y) / sphere->radius;
    n.z = (intersection.z - sphere->position.z) / sphere->radius;


    return n;
}
