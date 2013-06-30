/* 
 * File:   Structures_Definitions.h
 * Author: jose
 *
 * Created on June 3, 2013, 9:22 PM
 */

#ifndef STRUCTURES_DEFINITIONS_H
#define	STRUCTURES_DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#define EPSILON 0.0005 //This variable defines if the value of a calculation is equivalent to 0

    typedef enum {
        LIGHT = 0,
        SPHERE = 1,
        POLYGON = 2,
        DISK = 3,
        CYLINDER = 4,
        CONE = 5,
        PLANE = 6
    } OBJECT_TYPE;

    typedef struct {
        long double x;
        long double y;
        long double z;
    } VECTOR;

    typedef struct {
        GLfloat r;
        GLfloat g;
        GLfloat b;
    } RGB;

    typedef struct {
        long double x;
        long double y;
        long double z;
    } POINT;

    typedef struct {
        long double radius;
        VECTOR position;
    } OBJECT_SPHERE;

    typedef struct {
        VECTOR position;
        long double intensity_Ip;
        long double c1;
        long double c2;
        long double c3;
    } LIGHT_POINT;

    typedef struct {
        void* listOfPointsPtr;
        void* listOfPointsSplashPtr;
    } OBJECT_POLYGON;

    typedef struct {
        long double radiusOne;
        long double radiusTwo;
        VECTOR position;
        POINT point1;
        POINT point2;
        POINT point3;
    } OBJECT_DISK;
    
    typedef struct {
        long double radius;
        VECTOR position;
        POINT point1;
        POINT point2;
        POINT point3;
        VECTOR vectorQ;
        long double componentD;
        long double distance1;
        long double distance2;
    } OBJECT_CYLINDER;
    
    typedef struct {
        VECTOR position;
        POINT point1;
        POINT point2;
        POINT point3;
        VECTOR vectorQ;
        long double componentD;
        long double distance1;
        long double distance2;
        long double uRatio;
        long double vRatio;
    } OBJECT_CONE;

    typedef struct {
        RGB color;
        long double diffusionCoefficient_Kd; //Diffuse reflexion
        long double ambientLightingCoefficient_Ka; //Environment light
        long double specularReflexionCoeffient_Ks;
        long double specularReflexionRefinement_Kn;
        long double O1; //This variable controls the quantity of color takes of the object for the mirrors
        long double O2; //This variable controls the quantity of color takes from the reflexion for the mirrors
        long double O3; //This variable control the transparency of the object
        long double translucencyCoefficient_kt;
        long double componentD;
        long double dependedComponentD;
        OBJECT_TYPE type;
        OBJECT_SPHERE sphere;
        OBJECT_POLYGON polygon;
        OBJECT_DISK disk;
        OBJECT_CYLINDER cylinder;
        OBJECT_CONE cone;
        VECTOR normal;          //Original Normal
        VECTOR dependedNormal;  //Norm used by methods 
    } OBJECT;

    typedef struct {
        OBJECT object;
        long double t; //Represent the list of intersections, the first one is the nearest one
        VECTOR intersection;
    } INTERSECTION;

    typedef struct {
        OBJECT object;
        LIGHT_POINT light;
        POINT point;
        INTERSECTION intersection;
    } NODE_DATA;

    struct listNode {
        NODE_DATA data;
        struct listNode *nextPtr;
    };

    typedef struct listNode LIST_NODE;
    typedef LIST_NODE *LIST_NODE_PTR;



    /*******Global Variables Section*********/
    RGB backgroundColor;

#ifdef	__cplusplus
}
#endif

#endif	/* STRUCTURES_DEFINITIONS_H */

