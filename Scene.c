#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Polygon.h"

LIST_NODE_PTR sceneList = NULL; //The scene is an array of objects
LIST_NODE_PTR lightsList = NULL;

char *elementToken = ";";
char *pointsToken = "|";
char *pointToken = ",";

void readScene(char* fileName) {
    FILE *entry_file;
    OBJECT_TYPE type;
    NODE_DATA currentNode;
    long double distance; //Use to calculate VectorQ 

    char textFileLine[MAX_LINE_LENGTH]; //This represent the line on the text file

    char* line; //Copy of the current line

    if ((entry_file = fopen(fileName, "r")) == NULL) {//Reads the file, throws error if the file could not be opened
        perror(fileName);
    }

    //Read the scene Globals
    fgets(textFileLine, MAX_LINE_LENGTH, entry_file); //Read the line with the values

    line = strdup(textFileLine); //Copy the line
    //Set the position of the window
    xMin = atoi(strsep(&line, elementToken));
    yMin = atoi(strsep(&line, elementToken));
    xMax = atoi(strsep(&line, elementToken));
    yMax = atoi(strsep(&line, elementToken));

    //Set the position of the eye
    eyePosition.x = atof(strsep(&line, elementToken));
    eyePosition.y = atof(strsep(&line, elementToken));
    eyePosition.z = atof(strsep(&line, elementToken)); //This value is usually negative in order to see positive values of z

    //Set the background color
    backgroundColor.r = atof(strsep(&line, elementToken));
    backgroundColor.g = atof(strsep(&line, elementToken));
    backgroundColor.b = atof(strsep(&line, elementToken));

    ambientLighting_IA = atof(strsep(&line, elementToken));
    
    mirrorLevels = atof(strsep(&line, elementToken));

    free(line);


    while (fgets(textFileLine, 1000, entry_file) != NULL) {
        line = strdup(textFileLine); //Copy the line
        type = atoi(strsep(&line, elementToken));
        switch (type) {
            case SPHERE:
                currentNode.object.type = SPHERE;
                currentNode.object.color.r = atof(strsep(&line, elementToken));
                currentNode.object.color.g = atof(strsep(&line, elementToken));
                currentNode.object.color.b = atof(strsep(&line, elementToken));
                currentNode.object.diffusionCoefficient_Kd = atof(strsep(&line, elementToken));
                currentNode.object.ambientLightingCoefficient_Ka = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionCoeffient_Ks = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionRefinement_Kn = atof(strsep(&line, elementToken));
                currentNode.object.O1 = atof(strsep(&line, elementToken));
                currentNode.object.O2 = atof(strsep(&line, elementToken));
                currentNode.object.O3 = atof(strsep(&line, elementToken));
                currentNode.object.translucencyCoefficient_kt = atof(strsep(&line, elementToken));
                currentNode.object.sphere.radius = atof(strsep(&line, elementToken));
                currentNode.object.sphere.position.x = atof(strsep(&line, elementToken));
                currentNode.object.sphere.position.y = atof(strsep(&line, elementToken));
                currentNode.object.sphere.position.z = atof(strsep(&line, elementToken));

                insert(&sceneList, currentNode);

                break;
            case POLYGON:
                currentNode.object.type = POLYGON;
                currentNode.object.color.r = atof(strsep(&line, elementToken));
                currentNode.object.color.g = atof(strsep(&line, elementToken));
                currentNode.object.color.b = atof(strsep(&line, elementToken));
                currentNode.object.diffusionCoefficient_Kd = atof(strsep(&line, elementToken));
                currentNode.object.ambientLightingCoefficient_Ka = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionCoeffient_Ks = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionRefinement_Kn = atof(strsep(&line, elementToken));
                currentNode.object.O1 = atof(strsep(&line, elementToken));
                currentNode.object.O2 = atof(strsep(&line, elementToken));
                currentNode.object.O3 = atof(strsep(&line, elementToken));
                currentNode.object.translucencyCoefficient_kt = atof(strsep(&line, elementToken));

                LIST_NODE_PTR listPoints = NULL;
                LIST_NODE_PTR uvPointList;
                NODE_DATA currentNodePoint;
                char *currentPoint;
                while ((currentPoint = strsep(&line, pointsToken)) != NULL) {
                    currentNodePoint.point.x = atof(strsep(&currentPoint, pointToken));
                    currentNodePoint.point.y = atof(strsep(&currentPoint, pointToken));
                    currentNodePoint.point.z = atof(strsep(&currentPoint, pointToken));
                    insert(&listPoints, currentNodePoint);
                }
                
                getPlaneComponents(listPoints->data.point, listPoints->nextPtr->data.point, listPoints->nextPtr->nextPtr->data.point, &currentNode.object.normal, &currentNode.object.componentD);
                currentNode.object.dependedNormal = currentNode.object.normal;
                currentNode.object.dependedComponentD = currentNode.object.componentD;
                //I splash the polygon from the beginning
                uvPointList = splashPolygon(listPoints, &currentNode.object.normal);

                currentNode.object.polygon.listOfPointsPtr = listPoints;
                currentNode.object.polygon.listOfPointsSplashPtr = uvPointList;
                
                insert(&sceneList, currentNode);

                break;
            case LIGHT:
                currentNode.light.position.x = atof(strsep(&line, elementToken));
                currentNode.light.position.y = atof(strsep(&line, elementToken));
                currentNode.light.position.z = atof(strsep(&line, elementToken));
                currentNode.light.intensity_Ip = atof(strsep(&line, elementToken));
                currentNode.light.c1 = atof(strsep(&line, elementToken));
                currentNode.light.c2 = atof(strsep(&line, elementToken));
                currentNode.light.c3 = atof(strsep(&line, elementToken));

                insert(&lightsList, currentNode);
                break;
            case DISK:
                currentNode.object.type = DISK;
                currentNode.object.color.r = atof(strsep(&line, elementToken));
                currentNode.object.color.g = atof(strsep(&line, elementToken));
                currentNode.object.color.b = atof(strsep(&line, elementToken));
                currentNode.object.diffusionCoefficient_Kd = atof(strsep(&line, elementToken));
                currentNode.object.ambientLightingCoefficient_Ka = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionCoeffient_Ks = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionRefinement_Kn = atof(strsep(&line, elementToken));
                currentNode.object.O1 = atof(strsep(&line, elementToken));
                currentNode.object.O2 = atof(strsep(&line, elementToken));
                currentNode.object.O3 = atof(strsep(&line, elementToken));
                currentNode.object.translucencyCoefficient_kt = atof(strsep(&line, elementToken));
                
                currentNode.object.disk.radiusOne = atof(strsep(&line, elementToken));
                currentNode.object.disk.radiusTwo = atof(strsep(&line, elementToken));
                currentNode.object.disk.position.x = atof(strsep(&line, elementToken));
                currentNode.object.disk.position.y = atof(strsep(&line, elementToken));
                currentNode.object.disk.position.z = atof(strsep(&line, elementToken));
                
                currentNode.object.disk.point1.x = atof(strsep(&line, elementToken));
                currentNode.object.disk.point1.y = atof(strsep(&line, elementToken));
                currentNode.object.disk.point1.z = atof(strsep(&line, elementToken));
                
                currentNode.object.normal.x = currentNode.object.disk.position.x - currentNode.object.disk.point1.x;
                currentNode.object.normal.y = currentNode.object.cylinder.position.y - currentNode.object.disk.point1.y;
                currentNode.object.normal.z = currentNode.object.disk.position.z - currentNode.object.disk.point1.z;
                
                POINT point1, point2, point3;
                point1.x = 300;
                point1.y = 0;
                point1.z = 100;
                
                point2.x = 400;
                point2.y = 0;
                point2.z = 100;
                
                point3.x = 350;
                point3.y = 0;
                point3.z = 200;
                
                getPlaneComponents(point1, point2, point3, &currentNode.object.normal, &currentNode.object.componentD);
                //getComponentD(&currentNode.object.normal, &currentNode.object.disk.position, &currentNode.object.componentD);
                currentNode.object.dependedNormal = currentNode.object.normal;
                currentNode.object.dependedComponentD = currentNode.object.componentD;
                insert(&sceneList, currentNode);
                
                break;
            case CYLINDER:
                currentNode.object.type = CYLINDER;
                currentNode.object.color.r = atof(strsep(&line, elementToken));
                currentNode.object.color.g = atof(strsep(&line, elementToken));
                currentNode.object.color.b = atof(strsep(&line, elementToken));
                currentNode.object.diffusionCoefficient_Kd = atof(strsep(&line, elementToken));
                currentNode.object.ambientLightingCoefficient_Ka = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionCoeffient_Ks = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionRefinement_Kn = atof(strsep(&line, elementToken));
                currentNode.object.O1 = atof(strsep(&line, elementToken));
                currentNode.object.O2 = atof(strsep(&line, elementToken));
                currentNode.object.O3 = atof(strsep(&line, elementToken));
                currentNode.object.translucencyCoefficient_kt = atof(strsep(&line, elementToken));
                
                currentNode.object.cylinder.radius = atof(strsep(&line, elementToken));
                currentNode.object.cylinder.distance1 = atof(strsep(&line, elementToken));
                currentNode.object.cylinder.distance2 = atof(strsep(&line, elementToken));
                currentNode.object.cylinder.position.x = atof(strsep(&line, elementToken));
                currentNode.object.cylinder.position.y = atof(strsep(&line, elementToken));
                currentNode.object.cylinder.position.z = atof(strsep(&line, elementToken));
                
                currentNode.object.cylinder.point1.x = atof(strsep(&line, elementToken));
                currentNode.object.cylinder.point1.y = atof(strsep(&line, elementToken));
                currentNode.object.cylinder.point1.z = atof(strsep(&line, elementToken));
                
                currentNode.object.cylinder.vectorQ.x = currentNode.object.cylinder.position.x - currentNode.object.cylinder.point1.x;
                currentNode.object.cylinder.vectorQ.y = currentNode.object.cylinder.position.y - currentNode.object.cylinder.point1.y;
                currentNode.object.cylinder.vectorQ.z = currentNode.object.cylinder.position.z - currentNode.object.cylinder.point1.z;
                
                distance = calculateNormOfVector(&currentNode.object.cylinder.vectorQ);
                
                currentNode.object.cylinder.vectorQ.x = currentNode.object.cylinder.vectorQ.x / distance;
                currentNode.object.cylinder.vectorQ.y = currentNode.object.cylinder.vectorQ.y / distance;
                currentNode.object.cylinder.vectorQ.z = currentNode.object.cylinder.vectorQ.z / distance;

                insert(&sceneList, currentNode);
                
                break;
            case CONE:
                currentNode.object.type = CONE;
                currentNode.object.color.r = atof(strsep(&line, elementToken));
                currentNode.object.color.g = atof(strsep(&line, elementToken));
                currentNode.object.color.b = atof(strsep(&line, elementToken));
                currentNode.object.diffusionCoefficient_Kd = atof(strsep(&line, elementToken));
                currentNode.object.ambientLightingCoefficient_Ka = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionCoeffient_Ks = atof(strsep(&line, elementToken));
                currentNode.object.specularReflexionRefinement_Kn = atof(strsep(&line, elementToken));
                currentNode.object.O1 = atof(strsep(&line, elementToken));
                currentNode.object.O2 = atof(strsep(&line, elementToken));
                currentNode.object.O3 = atof(strsep(&line, elementToken));
                currentNode.object.translucencyCoefficient_kt = atof(strsep(&line, elementToken));
                
                currentNode.object.cone.uRatio = atof(strsep(&line, elementToken));
                currentNode.object.cone.vRatio = atof(strsep(&line, elementToken));
                currentNode.object.cone.distance1 = atof(strsep(&line, elementToken));
                currentNode.object.cone.distance2 = atof(strsep(&line, elementToken));
                currentNode.object.cone.position.x = atof(strsep(&line, elementToken));
                currentNode.object.cone.position.y = atof(strsep(&line, elementToken));
                currentNode.object.cone.position.z = atof(strsep(&line, elementToken));
                
                currentNode.object.cone.point1.x = atof(strsep(&line, elementToken));
                currentNode.object.cone.point1.y = atof(strsep(&line, elementToken));
                currentNode.object.cone.point1.z = atof(strsep(&line, elementToken));
                
                currentNode.object.cone.vectorQ.x = currentNode.object.cone.position.x - currentNode.object.cone.point1.x;
                currentNode.object.cone.vectorQ.y = currentNode.object.cone.position.y - currentNode.object.cone.point1.y;
                currentNode.object.cone.vectorQ.z = currentNode.object.cone.position.z - currentNode.object.cone.point1.z;
                
                distance = calculateNormOfVector(&currentNode.object.cone.vectorQ);
                
                currentNode.object.cone.vectorQ.x = currentNode.object.cone.vectorQ.x / distance;
                currentNode.object.cone.vectorQ.y = currentNode.object.cone.vectorQ.y / distance;
                currentNode.object.cone.vectorQ.z = currentNode.object.cone.vectorQ.z / distance;
                insert(&sceneList, currentNode);
                
                break;
        }

    }


    fclose(entry_file);
}

void getGeneralObjectInfo(char *line, NODE_DATA *currentNode) {
    (*currentNode).object.color.r = atof(strsep(&line, elementToken));
    (*currentNode).object.color.g = atof(strsep(&line, elementToken));
    (*currentNode).object.color.b = atof(strsep(&line, elementToken));
    (*currentNode).object.diffusionCoefficient_Kd = atof(strsep(&line, elementToken));
    (*currentNode).object.ambientLightingCoefficient_Ka = atof(strsep(&line, elementToken));
    (*currentNode).object.specularReflexionCoeffient_Ks = atof(strsep(&line, elementToken));
    (*currentNode).object.specularReflexionRefinement_Kn = atof(strsep(&line, elementToken));
}

