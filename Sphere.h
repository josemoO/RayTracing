/* 
 * File:   Sphere.h
 * Author: jose
 *
 * Created on June 3, 2013, 9:18 PM
 */

#ifndef SPHERE_H
#define	SPHERE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Structures_Definitions.h"
#include "Lists.h"


    void sphereIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *sphere, LIST_NODE_PTR *intersections);
    VECTOR calculateSphereNorm(VECTOR intersection, OBJECT_SPHERE *sphere);

#ifdef	__cplusplus
}
#endif

#endif	/* SPHERE_H */

