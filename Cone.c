#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Cone.h"
#include <values.h>

void coneInterseption(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, OBJECT *cone, LIST_NODE_PTR *intersections) {

    INTERSECTION intersectionOne, intersectionTwo;
    int insertIntersectionOne = 0, insertIntersectionTwo = 0;

    //precalculos para la ecuacion
    double a_partCommon, b_partCommon;
    double ax, bx, cx, ay, by, cy, az, bz, cz;
    double aRatio, bRatio, cRatio;


    //en el despeje esta parte es comun a los calculos que se haran despues
    a_partCommon = rayFromEyeDirection->x * cone->cone.vectorQ.x + rayFromEyeDirection->y * cone->cone.vectorQ.y + rayFromEyeDirection->z * cone->cone.vectorQ.z;

    b_partCommon = cone->cone.vectorQ.x * (eyePosition->x - cone->cone.position.x) +
            cone->cone.vectorQ.y * (eyePosition->y - cone->cone.position.y) +
            cone->cone.vectorQ.z * (eyePosition->z - cone->cone.position.z);

    //calculos de a               *****************************************************************************************
    ax = -2 * rayFromEyeDirection->x * (a_partCommon * cone->cone.vectorQ.x) +
            pow(a_partCommon * cone->cone.vectorQ.x, 2) + pow(rayFromEyeDirection->x, 2);
    ay = -2 * rayFromEyeDirection->y * (a_partCommon * cone->cone.vectorQ.y) +
            pow(a_partCommon * cone->cone.vectorQ.y, 2) + pow(rayFromEyeDirection->y, 2);
    az = -2 * rayFromEyeDirection->z * (a_partCommon * cone->cone.vectorQ.z) +
            pow(a_partCommon * cone->cone.vectorQ.z, 2) + pow(rayFromEyeDirection->z, 2);

    //calculos de b                 ****************************************************************************************
    bx = 2 * rayFromEyeDirection->x * eyePosition->x - 2 * rayFromEyeDirection->x * cone->cone.position.x - 2 * rayFromEyeDirection->x * (b_partCommon * cone->cone.vectorQ.x)
            - 2 * (a_partCommon * cone->cone.vectorQ.x) * eyePosition->x + 2 * (a_partCommon * cone->cone.vectorQ.x) * cone->cone.position.x + 2 * (a_partCommon * cone->cone.vectorQ.x) * (b_partCommon * cone->cone.vectorQ.x);

    by = 2 * rayFromEyeDirection->y * eyePosition->y - 2 * rayFromEyeDirection->y * cone->cone.position.y - 2 * rayFromEyeDirection->y * (b_partCommon * cone->cone.vectorQ.y)
            - 2 * (a_partCommon * cone->cone.vectorQ.y) * eyePosition->y + 2 * (a_partCommon * cone->cone.vectorQ.y) * cone->cone.position.y + 2 * (a_partCommon * cone->cone.vectorQ.y) * (b_partCommon * cone->cone.vectorQ.y);

    bz = 2 * rayFromEyeDirection->z * eyePosition->z - 2 * rayFromEyeDirection->z * cone->cone.position.z - 2 * rayFromEyeDirection->z * (b_partCommon * cone->cone.vectorQ.z)
            - 2 * (a_partCommon * cone->cone.vectorQ.z) * eyePosition->z + 2 * (a_partCommon * cone->cone.vectorQ.z) * cone->cone.position.z + 2 * (a_partCommon * cone->cone.vectorQ.z) * (b_partCommon * cone->cone.vectorQ.z);

    //calculos de los c             *****************************************************************************************
    cx = pow(eyePosition->x, 2) + pow(cone->cone.position.x, 2) + pow(b_partCommon * cone->cone.vectorQ.x, 2)
            - 2 * eyePosition->x * cone->cone.position.x - 2 * eyePosition->x * (b_partCommon * cone->cone.vectorQ.x) + 2 * cone->cone.position.x * (b_partCommon * cone->cone.vectorQ.x);

    cy = pow(eyePosition->y, 2) + pow(cone->cone.position.y, 2) + pow(b_partCommon * cone->cone.vectorQ.y, 2)
            - 2 * eyePosition->y * cone->cone.position.y - 2 * eyePosition->y * (b_partCommon * cone->cone.vectorQ.y) + 2 * cone->cone.position.y * (b_partCommon * cone->cone.vectorQ.y);

    cz = pow(eyePosition->z, 2) + pow(cone->cone.position.z, 2) + pow(b_partCommon * cone->cone.vectorQ.z, 2)
            - 2 * eyePosition->z * cone->cone.position.z - 2 * eyePosition->z * (b_partCommon * cone->cone.vectorQ.z) + 2 * cone->cone.position.z * (b_partCommon * cone->cone.vectorQ.z);


    /******************************************/
    /*Calculo de la parte de la razon del cono*/
    double ratio2 = pow(cone->cone.vRatio / cone->cone.uRatio, 2);
    aRatio = pow(a_partCommon, 2) * ratio2;
    bRatio = 2 * a_partCommon * ratio2 * b_partCommon;
    cRatio = pow(b_partCommon, 2) * ratio2;
    /******************************************/

    //VARIABLES FINALES
    double a_final, b_final, c_final, discriminating, t1, t2;
    a_final = (ax + ay + az) - aRatio;
    b_final = (bx + by + bz) - bRatio;
    c_final = (cx + cy + cz) - cRatio;

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
            intersectionOne.object = *cone;

            if (cone->cone.distance1 != 0 || cone->cone.distance2 != 0) {

                double distanceOriginTo_XM_YM_ZM; //variable para el calculo de la distancia dentro de la misma linea

                //calculo de d para ver si esta en medio de h1 y h2
                distanceOriginTo_XM_YM_ZM = t1 * a_partCommon + b_partCommon;

                //si resulta que NO se encuentra en medio, ahora calculo con respecto a t2... ya que puedo estar viendo el interior del cilindro
                if (distanceOriginTo_XM_YM_ZM <= cone->cone.distance2 && distanceOriginTo_XM_YM_ZM >= cone->cone.distance1) {
                    insertIntersectionOne = 1;
                }

            } else {
                insertIntersectionOne = 1;
            }


            if (insertIntersectionOne == 1) {
                double x_temp = intersectionOne.intersection.x * pow(cone->cone.vectorQ.x, 2) * ratio2 -
                        pow(cone->cone.vectorQ.x, 2) * ratio2 * cone->cone.position.x +
                        cone->cone.vectorQ.x * cone->cone.vectorQ.y * ratio2 * intersectionOne.intersection.y -
                        cone->cone.vectorQ.x * cone->cone.vectorQ.y * ratio2 * cone->cone.position.y +
                        cone->cone.vectorQ.x * cone->cone.vectorQ.z * ratio2 * intersectionOne.intersection.z -
                        cone->cone.vectorQ.x * cone->cone.vectorQ.z * ratio2 * cone->cone.position.z;

                double y_temp = intersectionOne.intersection.y * pow(cone->cone.vectorQ.y, 2) * ratio2 -
                        pow(cone->cone.vectorQ.y, 2) * ratio2 * cone->cone.position.y +
                        cone->cone.vectorQ.x * cone->cone.vectorQ.y * ratio2 * intersectionOne.intersection.x -
                        cone->cone.vectorQ.x * cone->cone.vectorQ.y * ratio2 * cone->cone.position.x +
                        cone->cone.vectorQ.y * cone->cone.vectorQ.z * ratio2 * intersectionOne.intersection.z -
                        cone->cone.vectorQ.y * cone->cone.vectorQ.z * ratio2 * cone->cone.position.z;

                double z_temp = intersectionOne.intersection.z * pow(cone->cone.vectorQ.z, 2) * ratio2 -
                        pow(cone->cone.vectorQ.z, 2) * ratio2 * cone->cone.position.z +
                        cone->cone.vectorQ.x * cone->cone.vectorQ.z * ratio2 * intersectionOne.intersection.x -
                        cone->cone.vectorQ.x * cone->cone.vectorQ.z * ratio2 * cone->cone.position.x +
                        cone->cone.vectorQ.y * cone->cone.vectorQ.z * ratio2 * intersectionOne.intersection.y -
                        cone->cone.vectorQ.y * cone->cone.vectorQ.z * ratio2 * cone->cone.position.y;


                intersectionOne.object.normal.x = (intersectionOne.intersection.x -
                        (cone->cone.position.x + cone->cone.vectorQ.x * (intersectionOne.t * a_partCommon + b_partCommon))) - x_temp;
                intersectionOne.object.normal.y = (intersectionOne.intersection.y -
                        (cone->cone.position.y + cone->cone.vectorQ.y * (intersectionOne.t * a_partCommon + b_partCommon))) - y_temp;
                intersectionOne.object.normal.z = (intersectionOne.intersection.z -
                        (cone->cone.position.z + cone->cone.vectorQ.z * (intersectionOne.t * a_partCommon + b_partCommon))) - z_temp;

                //para hacerlo unitario
                double norm = sqrt(pow(intersectionOne.object.normal.x, 2) + pow(intersectionOne.object.normal.y, 2) +
                        pow(intersectionOne.object.normal.z, 2));

                intersectionOne.object.normal.x = intersectionOne.object.normal.x / norm;
                intersectionOne.object.normal.y = intersectionOne.object.normal.y / norm;
                intersectionOne.object.normal.z = intersectionOne.object.normal.z / norm;
                
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
            intersectionTwo.object = *cone;

            if (cone->cone.distance1 != 0 || cone->cone.distance2 != 0) {

                double distanceOriginTo_XM_YM_ZM; //variable para el calculo de la distancia dentro de la misma linea

                //calculo de d para ver si esta en medio de h1 y h2
                distanceOriginTo_XM_YM_ZM = t2 * a_partCommon + b_partCommon;

                //si resulta que NO se encuentra en medio, ahora calculo con respecto a t2... ya que puedo estar viendo el interior del cilindro
                if (distanceOriginTo_XM_YM_ZM <= cone->cone.distance2 && distanceOriginTo_XM_YM_ZM >= cone->cone.distance1) {
                    insertIntersectionTwo = 1;
                }

            } else {
                insertIntersectionTwo = 1;
            }


            if (insertIntersectionTwo == 1) {
                double x_temp = intersectionTwo.intersection.x * pow(cone->cone.vectorQ.x, 2) * ratio2 -
                        pow(cone->cone.vectorQ.x, 2) * ratio2 * cone->cone.position.x +
                        cone->cone.vectorQ.x * cone->cone.vectorQ.y * ratio2 * intersectionTwo.intersection.y -
                        cone->cone.vectorQ.x * cone->cone.vectorQ.y * ratio2 * cone->cone.position.y +
                        cone->cone.vectorQ.x * cone->cone.vectorQ.z * ratio2 * intersectionTwo.intersection.z -
                        cone->cone.vectorQ.x * cone->cone.vectorQ.z * ratio2 * cone->cone.position.z;

                double y_temp = intersectionTwo.intersection.y * pow(cone->cone.vectorQ.y, 2) * ratio2 -
                        pow(cone->cone.vectorQ.y, 2) * ratio2 * cone->cone.position.y +
                        cone->cone.vectorQ.x * cone->cone.vectorQ.y * ratio2 * intersectionTwo.intersection.x -
                        cone->cone.vectorQ.x * cone->cone.vectorQ.y * ratio2 * cone->cone.position.x +
                        cone->cone.vectorQ.y * cone->cone.vectorQ.z * ratio2 * intersectionTwo.intersection.z -
                        cone->cone.vectorQ.y * cone->cone.vectorQ.z * ratio2 * cone->cone.position.z;

                double z_temp = intersectionTwo.intersection.z * pow(cone->cone.vectorQ.z, 2) * ratio2 -
                        pow(cone->cone.vectorQ.z, 2) * ratio2 * cone->cone.position.z +
                        cone->cone.vectorQ.x * cone->cone.vectorQ.z * ratio2 * intersectionTwo.intersection.x -
                        cone->cone.vectorQ.x * cone->cone.vectorQ.z * ratio2 * cone->cone.position.x +
                        cone->cone.vectorQ.y * cone->cone.vectorQ.z * ratio2 * intersectionTwo.intersection.y -
                        cone->cone.vectorQ.y * cone->cone.vectorQ.z * ratio2 * cone->cone.position.y;


                intersectionTwo.object.normal.x = (intersectionTwo.intersection.x -
                        (cone->cone.position.x + cone->cone.vectorQ.x * (intersectionTwo.t * a_partCommon + b_partCommon))) - x_temp;
                intersectionTwo.object.normal.y = (intersectionTwo.intersection.y -
                        (cone->cone.position.y + cone->cone.vectorQ.y * (intersectionTwo.t * a_partCommon + b_partCommon))) - y_temp;
                intersectionTwo.object.normal.z = (intersectionTwo.intersection.z -
                        (cone->cone.position.z + cone->cone.vectorQ.z * (intersectionTwo.t * a_partCommon + b_partCommon))) - z_temp;

                //para hacerlo unitario
                double norm = sqrt(pow(intersectionTwo.object.normal.x, 2) + pow(intersectionTwo.object.normal.y, 2) +
                        pow(intersectionTwo.object.normal.z, 2));

                intersectionTwo.object.normal.x = intersectionTwo.object.normal.x / norm;
                intersectionTwo.object.normal.y = intersectionTwo.object.normal.y / norm;
                intersectionTwo.object.normal.z = intersectionTwo.object.normal.z / norm;
                
                intersectionTwo.object.dependedNormal = intersectionTwo.object.normal;

                insertInOrder(intersections, intersectionTwo);
            }

        }
    }//end   if (discriminating > EPSILON)
}
