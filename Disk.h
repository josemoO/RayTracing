/* 
 * File:   Disk.h
 * Author: jose
 *
 * Created on June 14, 2013, 9:27 PM
 */

#ifndef DISK_H
#define	DISK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Structures_Definitions.h"
#include "Lists.h"

    void diskIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *disk, LIST_NODE_PTR *intersections);

#ifdef	__cplusplus
}
#endif

#endif	/* DISK_H */

