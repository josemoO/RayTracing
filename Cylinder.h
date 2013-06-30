/* 
 * File:   Cylinder.h
 * Author: jose
 *
 * Created on June 16, 2013, 11:04 PM
 */

#ifndef CYLINDER_H
#define	CYLINDER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Structures_Definitions.h"
#include "Lists.h"

void cylinderInterseption(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *cylinder, LIST_NODE_PTR *intersections);

#ifdef	__cplusplus
}
#endif

#endif	/* CYLINDER_H */

