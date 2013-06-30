#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Cylinder.h"
#include "Utils.h"
#include "Lists.h"
#include <values.h>

void cylinderInterseption(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *cylinder, LIST_NODE_PTR *intersections) {
    INTERSECTION intersectionOne, intersectionTwo;

    //precalculos para la ecuacion
    double a_partCommon, b_partCommon;
    double ax, bx, cx, ay, by, cy, az, bz, cz;

    int insertIntersectionOne = 0, insertIntersectionTwo = 0;

    //en el despeje esta parte es comun a los calculos que se haran despues
    a_partCommon = rayFromEyeDirection->x * cylinder->cylinder.vectorQ.x + rayFromEyeDirection->y * cylinder->cylinder.vectorQ.y + rayFromEyeDirection->z * cylinder->cylinder.vectorQ.z;

    b_partCommon = cylinder->cylinder.vectorQ.x * (eyePosition->x - cylinder->cylinder.position.x) +
            cylinder->cylinder.vectorQ.y * (eyePosition->y - cylinder->cylinder.position.y) +
            cylinder->cylinder.vectorQ.z * (eyePosition->z - cylinder->cylinder.position.z);


    //calculos de a               *****************************************************************************************
    ax = -2 * rayFromEyeDirection->x * (a_partCommon * cylinder->cylinder.vectorQ.x) +
            pow(a_partCommon * cylinder->cylinder.vectorQ.x, 2) + pow(rayFromEyeDirection->x, 2);
    ay = -2 * rayFromEyeDirection->y * (a_partCommon * cylinder->cylinder.vectorQ.y) +
            pow(a_partCommon * cylinder->cylinder.vectorQ.y, 2) + pow(rayFromEyeDirection->y, 2);
    az = -2 * rayFromEyeDirection->z * (a_partCommon * cylinder->cylinder.vectorQ.z) +
            pow(a_partCommon * cylinder->cylinder.vectorQ.z, 2) + pow(rayFromEyeDirection->z, 2);

    //calculos de b                 ****************************************************************************************
    bx = 2 * rayFromEyeDirection->x * eyePosition->x - 2 * rayFromEyeDirection->x * cylinder->cylinder.position.x - 2 * rayFromEyeDirection->x * (b_partCommon * cylinder->cylinder.vectorQ.x)
            - 2 * (a_partCommon * cylinder->cylinder.vectorQ.x) * eyePosition->x + 2 * (a_partCommon * cylinder->cylinder.vectorQ.x) * cylinder->cylinder.position.x + 2 * (a_partCommon * cylinder->cylinder.vectorQ.x) * (b_partCommon * cylinder->cylinder.vectorQ.x);

    by = 2 * rayFromEyeDirection->y * eyePosition->y - 2 * rayFromEyeDirection->y * cylinder->cylinder.position.y - 2 * rayFromEyeDirection->y * (b_partCommon * cylinder->cylinder.vectorQ.y)
            - 2 * (a_partCommon * cylinder->cylinder.vectorQ.y) * eyePosition->y + 2 * (a_partCommon * cylinder->cylinder.vectorQ.y) * cylinder->cylinder.position.y + 2 * (a_partCommon * cylinder->cylinder.vectorQ.y) * (b_partCommon * cylinder->cylinder.vectorQ.y);

    bz = 2 * rayFromEyeDirection->z * eyePosition->z - 2 * rayFromEyeDirection->z * cylinder->cylinder.position.z - 2 * rayFromEyeDirection->z * (b_partCommon * cylinder->cylinder.vectorQ.z)
            - 2 * (a_partCommon * cylinder->cylinder.vectorQ.z) * eyePosition->z + 2 * (a_partCommon * cylinder->cylinder.vectorQ.z) * cylinder->cylinder.position.z + 2 * (a_partCommon * cylinder->cylinder.vectorQ.z) * (b_partCommon * cylinder->cylinder.vectorQ.z);

    //calculos de los c             *****************************************************************************************
    cx = pow(eyePosition->x, 2) + pow(cylinder->cylinder.position.x, 2) + pow(b_partCommon * cylinder->cylinder.vectorQ.x, 2)
            - 2 * eyePosition->x * cylinder->cylinder.position.x - 2 * eyePosition->x * (b_partCommon * cylinder->cylinder.vectorQ.x) + 2 * cylinder->cylinder.position.x * (b_partCommon * cylinder->cylinder.vectorQ.x);

    cy = pow(eyePosition->y, 2) + pow(cylinder->cylinder.position.y, 2) + pow(b_partCommon * cylinder->cylinder.vectorQ.y, 2)
            - 2 * eyePosition->y * cylinder->cylinder.position.y - 2 * eyePosition->y * (b_partCommon * cylinder->cylinder.vectorQ.y) + 2 * cylinder->cylinder.position.y * (b_partCommon * cylinder->cylinder.vectorQ.y);

    cz = pow(eyePosition->z, 2) + pow(cylinder->cylinder.position.z, 2) + pow(b_partCommon * cylinder->cylinder.vectorQ.z, 2)
            - 2 * eyePosition->z * cylinder->cylinder.position.z - 2 * eyePosition->z * (b_partCommon * cylinder->cylinder.vectorQ.z) + 2 * cylinder->cylinder.position.z * (b_partCommon * cylinder->cylinder.vectorQ.z);


    //VARIABLES FINALES
    double a_final, b_final, c_final, discriminating, t1, t2;
    a_final = ax + ay + az;
    b_final = bx + by + bz;
    c_final = (cx + cy + cz) - pow(cylinder->cylinder.radius, 2);


    //DE ESTA FORMULA FINAL, HAGO EL CALCULO DEL DISCRIMINANTE
    discriminating = pow(b_final, 2) - 4 * a_final * c_final;

    //preguntar si existen soluciones
    if (discriminating > EPSILON) {
        //CALCULO T1 y T2
        t1 = (-1 * b_final + sqrt(discriminating)) / (2 * a_final);
        t2 = (-1 * b_final - sqrt(discriminating)) / (2 * a_final);

        //pregunto si el cilindro esta delante del ojo
        if (t1 > EPSILON) {

            //calculo el lugar exacto de la interseccion
            intersectionOne.intersection.x = eyePosition->x + t1 * rayFromEyeDirection->x;
            intersectionOne.intersection.y = eyePosition->y + t1 * rayFromEyeDirection->y;
            intersectionOne.intersection.z = eyePosition->z + t1 * rayFromEyeDirection->z;
            intersectionOne.t = t1;
            intersectionOne.object = *cylinder;

            if (cylinder->cylinder.distance1 != 0 || cylinder->cylinder.distance2 != 0) {

                double distanceOriginTo_XM_YM_ZM; //variable para el calculo de la distancia dentro de la misma linea

                //calculo de d para ver si esta en medio de h1 y h2
                distanceOriginTo_XM_YM_ZM = t1 * a_partCommon + b_partCommon;

                //si resulta que NO se encuentra en medio, ahora calculo con respecto a t2... ya que puedo estar viendo el interior del cilindro
                if (distanceOriginTo_XM_YM_ZM <= cylinder->cylinder.distance2 && distanceOriginTo_XM_YM_ZM >= cylinder->cylinder.distance1) {
                    insertIntersectionOne = 1;
                }

            } else {
                insertIntersectionOne = 1;
            }


            if (insertIntersectionOne == 1) {
                //calculo la normal
                intersectionOne.object.normal.x = (intersectionOne.intersection.x -
                        (cylinder->cylinder.position.x + cylinder->cylinder.vectorQ.x * (intersectionOne.t * a_partCommon + b_partCommon))) / cylinder->cylinder.radius;
                intersectionOne.object.normal.y = (intersectionOne.intersection.y -
                        (cylinder->cylinder.position.y + cylinder->cylinder.vectorQ.y * (intersectionOne.t * a_partCommon + b_partCommon))) / cylinder->cylinder.radius;
                intersectionOne.object.normal.z = (intersectionOne.intersection.z -
                        (cylinder->cylinder.position.z + cylinder->cylinder.vectorQ.z * (intersectionOne.t * a_partCommon + b_partCommon))) / cylinder->cylinder.radius;

                intersectionOne.object.dependedNormal = intersectionOne.object.normal;

                insertInOrder(intersections, intersectionOne);
            }

        }
        
        if (t2 > EPSILON) {

            //calculo el lugar exacto de la interseccion
            intersectionTwo.intersection.x = eyePosition->x + t2 * rayFromEyeDirection->x;
            intersectionTwo.intersection.y = eyePosition->y + t2 * rayFromEyeDirection->y;
            intersectionTwo.intersection.z = eyePosition->z + t2 * rayFromEyeDirection->z;
            intersectionTwo.t = t2;
            intersectionTwo.object = *cylinder;

            if (cylinder->cylinder.distance1 != 0 || cylinder->cylinder.distance2 != 0) {

                double distanceOriginTo_XM_YM_ZM; //variable para el calculo de la distancia dentro de la misma linea

                //calculo de d para ver si esta en medio de h1 y h2
                distanceOriginTo_XM_YM_ZM = t2 * a_partCommon + b_partCommon;

                //si resulta que NO se encuentra en medio, ahora calculo con respecto a t2... ya que puedo estar viendo el interior del cilindro
                if (distanceOriginTo_XM_YM_ZM <= cylinder->cylinder.distance2 && distanceOriginTo_XM_YM_ZM >= cylinder->cylinder.distance1) {
                    insertIntersectionTwo = 1;
                }

            } else {
                insertIntersectionTwo = 1;
            }


            if (insertIntersectionTwo == 1) {
                //calculo la normal
                intersectionTwo.object.normal.x = (intersectionTwo.intersection.x -
                        (cylinder->cylinder.position.x + cylinder->cylinder.vectorQ.x * (intersectionTwo.t * a_partCommon + b_partCommon))) / cylinder->cylinder.radius;
                intersectionTwo.object.normal.y = (intersectionTwo.intersection.y -
                        (cylinder->cylinder.position.y + cylinder->cylinder.vectorQ.y * (intersectionTwo.t * a_partCommon + b_partCommon))) / cylinder->cylinder.radius;
                intersectionTwo.object.normal.z = (intersectionTwo.intersection.z -
                        (cylinder->cylinder.position.z + cylinder->cylinder.vectorQ.z * (intersectionTwo.t * a_partCommon + b_partCommon))) / cylinder->cylinder.radius;

                intersectionTwo.object.dependedNormal = intersectionTwo.object.normal;

                insertInOrder(intersections, intersectionTwo);
            }

        }
    }// end   if (discriminating > EPSILON_ZERO)



}
