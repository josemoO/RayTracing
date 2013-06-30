/* 
 * File:   Cone.h
 * Author: jose
 *
 * Created on June 17, 2013, 8:35 AM
 */

#ifndef CONE_H
#define	CONE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Structures_Definitions.h"

void coneInterseption(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *cone, LIST_NODE_PTR *intersections);

#ifdef	__cplusplus
}
#endif

#endif	/* CONE_H */

