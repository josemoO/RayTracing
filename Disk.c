#include "Disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void diskIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *disk, LIST_NODE_PTR *intersections) {
    int wasIntersectionWithPlaneFound;
    INTERSECTION intersectionWithPlane, intersectionWithDisk;

    long double intersectionResult, radiousOne, radiousTwo;

    if (dotProduct(&rayFromEyeDirection, &disk->normal) < EPSILON) {
        VECTOR newN;
        newN.x = -1 * disk->normal.x;
        newN.y = -1 * disk->normal.y;
        newN.z = -1 * disk->normal.z;
        disk->dependedNormal = newN;
        disk->dependedComponentD = -1 * disk->componentD;
    } else {
        disk->dependedNormal = disk->normal;
        disk->dependedComponentD = disk->componentD;
    }

    calculatePlaneIntersection(eyePosition, rayFromEyeDirection, disk, &intersectionWithPlane, &wasIntersectionWithPlaneFound);

    if (wasIntersectionWithPlaneFound == 1) {
        intersectionResult = pow(disk->disk.position.x - intersectionWithPlane.intersection.x, 2) +
                pow(disk->disk.position.y - intersectionWithPlane.intersection.y, 2) +
                pow(disk->disk.position.z - intersectionWithPlane.intersection.z, 2);
        radiousOne = pow(disk->disk.radiusOne, 2);
        radiousTwo = pow(disk->disk.radiusTwo, 2);
        if (radiousOne <= intersectionResult && intersectionResult <= radiousTwo) {
            intersectionWithDisk.intersection = intersectionWithPlane.intersection;
            intersectionWithDisk.object = *disk;
            intersectionWithDisk.t = intersectionWithPlane.t;
            
            insertInOrder(intersections, intersectionWithDisk);
        }
    }
}
