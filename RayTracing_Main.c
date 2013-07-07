/* 
 * File:   main.c
 * Author: root
 *
 * Created on April 25, 2013, 5:56 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <values.h>

#include "Scene.h"
#include "Utils.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Disk.h"
#include "Cylinder.h"
#include "Cone.h"
#include <png.h>


#define APPLY_SHADOWS 1
#define APPLY_SPECULAR_REFLEXION 1
#define APPLY_MIRRORS 1
#define APPLY_ANTIALISING 1
#define APPLY_TRANSPARENCY 1
//These variables control the frame buffer size
#define H_RES 1008
#define V_RES 567

//Prototype functions
void display();
void map(int i, int j, VECTOR *pixelWindow, long double iDisplacement, long double jDisplacement);
RGB whichColor(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, int reflexionLevel, int *setBackground);
void iluminatePixel(VECTOR *anchor, VECTOR *rayFromEyeDirection, RGB *color, INTERSECTION *intersection);
void firstIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, LIST_NODE_PTR *intersections);
long double calculateIntensity(INTERSECTION intersection, VECTOR lightPoint);
VECTOR calculateVectorToLight_L(VECTOR intersection, VECTOR lightPoint, long double *distance);
long double attenuationFactor(VECTOR intersection, LIGHT_POINT light);
int save_png_to_file(const char *path);
RGB getAntialiasingColor(long double i, long double j, long double unit, int depth, int mirrors_level, int transparency_level);
void rayTracing();
RGB frameBuffer[H_RES][V_RES];

double testPointI = 350;
double testPointJ = 25;
int stop = 0;

/*
 * 
 */
int main(int argc, char** argv) {

    //Create the scene
    readScene("ModernHouse");

    rayTracing();

    save_png_to_file("image.png");

    //Create the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(H_RES, V_RES);
    glutInitWindowPosition(500, 200);
    glutCreateWindow("Ray tracing");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(0.0, H_RES, 0.0, V_RES);

    //Call to callback function
    glutDisplayFunc(display);

    glutMainLoop();




    display();
    return (EXIT_SUCCESS);
}

void rayTracing() {
    time_t startTimeBeen = time(NULL), endTimeBeen;
    struct tm startTime = *localtime(&startTimeBeen), endTime;
    double startTimeSeconds, endTimeSeconds, differenceSeconds, differenceMinutesRest, differenceHoursRest;
    int differenceMinutes, differenceHours;
    startTimeSeconds = startTime.tm_sec + (startTime.tm_min * 60) + (startTime.tm_hour * 60 * 60);

    printf("Start Time: %d-%d-%d %d:%d:%d\n", startTime.tm_year + 1900, startTime.tm_mon + 1, startTime.tm_mday, startTime.tm_hour, startTime.tm_min, startTime.tm_sec);
    int i, j; //Control the vertical and horizontal pixels

    for (i = 0; i < H_RES; i++) {
        for (j = 0; j < V_RES; j++) {
            if (i == testPointI && j == testPointJ) {
                stop = 1;
            }

            frameBuffer[i][j] = getAntialiasingColor((long double) i, (long double) j, 1, 5, mirrorLevels, 0);
        }
    }
    endTimeBeen = time(NULL);
    endTime = *localtime(&endTimeBeen);
    endTimeSeconds = endTime.tm_sec + (endTime.tm_min * 60) + (endTime.tm_hour * 60 * 60);
    printf("End Time: %d-%d-%d %d:%d:%d\n", endTime.tm_year + 1900, endTime.tm_mon + 1, endTime.tm_mday, endTime.tm_hour, endTime.tm_min, endTime.tm_sec);
    differenceSeconds = endTimeSeconds - startTimeSeconds;
    
    differenceHours = differenceSeconds / 60 / 60;
    differenceHoursRest = ( ((int) differenceSeconds / 60) % 60);
    differenceMinutes = (differenceSeconds - (differenceHours * 60 * 60)) / 60;
    differenceMinutesRest = ( (int) (differenceSeconds - (differenceHours * 60 * 60)) ) % 60;;
    
    
    
    printf("Time: %d:%d:%0.f\n", differenceHours, differenceMinutes, differenceMinutesRest);
    
}

void display(void) {
    glBegin(GL_POINTS);
    int i, j;
    for (i = 0; i < H_RES; i++) {
        for (j = 0; j < V_RES; j++) {

            glColor3f(frameBuffer[i][j].r, frameBuffer[i][j].g, frameBuffer[i][j].b);
            glVertex2i(i, j);
        }
    }
    /*

        glColor3f(1.0, 0.0, 0.0);
        glVertex2i(testPointI, testPointJ);
     */

    glEnd();
    glFlush(); //Reload the window    
}

RGB getAntialiasingColor(long double i, long double j, long double unit, int depth, int mirrors_level, int transparency_level) {
    //variables
    long double l_norma;
    RGB antialiasing_pixels[4];
    int times;
    long double iSum = 0, jSum = 0;

    RGB currentRGB;

    VECTOR pixelW; //Control the position of the pixel on the window

    VECTOR direction; //Represent the normalized form of the vector
    int isBackGround;

    //indica si usa antialiasing
    if (APPLY_ANTIALISING) {
        //cuatro rayos
        for (times = 0; times < 4; times++) {

            switch (times) {
                case 0:
                {
                    iSum = 0;
                    jSum = 0;
                    break;
                }
                case 1:
                {
                    iSum = 0;
                    jSum = unit;
                    break;
                }
                case 2:
                {
                    iSum = unit;
                    jSum = 0;
                    break;
                }
                case 3:
                {
                    iSum = unit;
                    jSum = unit;
                    break;
                }
            }

            //Map the values
            map(i, j, &pixelW, iSum, jSum);

            //I have to normalize the vector
            l_norma = calculateNormBetweenTwoVectors(&pixelW, &eyePosition);
            direction.x = (pixelW.x - eyePosition.x) / l_norma;
            direction.y = (pixelW.y - eyePosition.y) / l_norma;
            direction.z = (pixelW.z - eyePosition.z) / l_norma;

            //ya tengo el punto tridimensional en el espacio 3D
            //ahora necesito definir el rayo desde el cual el ojo esta viendo
            antialiasing_pixels[times] = whichColor(&eyePosition, &direction, mirrorLevels, &isBackGround);
        }//end for


        //solo permite una profundidad de 4
        if (depth > 0) {

            //DISTANCIAS EUCLIDIANAS CON RESPECTO AL PUNTO IZQUIERDO INFERIOR
            double euclideanDistance_0_1 = sqrt(pow(antialiasing_pixels[0].r - antialiasing_pixels[1].r, 2) +
                    pow(antialiasing_pixels[0].g - antialiasing_pixels[1].g, 2) +
                    pow(antialiasing_pixels[0].b - antialiasing_pixels[1].b, 2));

            double euclideanDistance_0_2 = sqrt(pow(antialiasing_pixels[0].r - antialiasing_pixels[2].r, 2) +
                    pow(antialiasing_pixels[0].g - antialiasing_pixels[2].g, 2) +
                    pow(antialiasing_pixels[0].b - antialiasing_pixels[2].b, 2));

            double euclideanDistance_0_3 = sqrt(pow(antialiasing_pixels[0].r - antialiasing_pixels[3].r, 2) +
                    pow(antialiasing_pixels[0].g - antialiasing_pixels[3].g, 2) +
                    pow(antialiasing_pixels[0].b - antialiasing_pixels[3].b, 2));

            //consulto las distancias euclidianas
            if (euclideanDistance_0_1 > 0.1) {
                antialiasing_pixels[1] = getAntialiasingColor(i, j + unit / 2, unit / 2, --depth, mirrors_level, transparency_level);
            }
            if (euclideanDistance_0_2 > 0.1) {
                antialiasing_pixels[2] = getAntialiasingColor(i + unit / 2, j, unit / 2, --depth, mirrors_level, transparency_level);
            }
            if (euclideanDistance_0_3 > 0.1) {
                antialiasing_pixels[0] = getAntialiasingColor(i, j, unit / 2, --depth, mirrors_level, transparency_level);
                antialiasing_pixels[3] = getAntialiasingColor(i + unit / 2, j + unit / 2, unit / 2, --depth, mirrors_level, transparency_level);
            }
        }

        currentRGB.r = antialiasing_pixels[0].r;
        currentRGB.g = antialiasing_pixels[0].g;
        currentRGB.b = antialiasing_pixels[0].b;
        //sumatoria de los 4 rayos
        for (times = 1; times < 4; times++) {
            currentRGB.r += antialiasing_pixels[times].r;
            currentRGB.g += antialiasing_pixels[times].g;
            currentRGB.b += antialiasing_pixels[times].b;
        }

        //promedio
        currentRGB.r = currentRGB.r / 4;
        currentRGB.g = currentRGB.g / 4;
        currentRGB.b = currentRGB.b / 4;
    }//end  if(withAntialiasing == 1)
    else {

        //Map the values
        map(i, j, &pixelW, 0.5, 0.5);

        //I have to normalize the vector
        l_norma = calculateNormBetweenTwoVectors(&pixelW, &eyePosition);
        direction.x = (pixelW.x - eyePosition.x) / l_norma;
        direction.y = (pixelW.y - eyePosition.y) / l_norma;
        direction.z = (pixelW.z - eyePosition.z) / l_norma;

        //Set the color
        currentRGB = whichColor(&eyePosition, &direction, mirrorLevels, &isBackGround);

    }

    return currentRGB;
}

/*
 * The map method transform the pixel in the frame buffer to the equivalent of
 * a pixel in the window. It receives four parameters:
 *  -i: The x value of the frame buffer.
 *  -j: The y value of the frame buffer.
 *  -*pixelWindow: This is the Vector who contains the position of the pixel on the window. This is an output parameter
 */
void map(int i, int j, VECTOR *pixelWindow, long double iDisplacement, long double jDisplacement) {
    VECTOR mapped;
    mapped.x = ((i + iDisplacement) * (xMax - xMin)) / (H_RES + xMin);
    mapped.y = ((j + jDisplacement) * (yMax - yMin)) / (V_RES + yMin);
    mapped.z = 0.0; //The z value of the window is always 0
    *pixelWindow = mapped;
}

RGB whichColor(VECTOR *anchor, VECTOR *rayFromEyeDirection, int reflexionLevel, int *setBackground) {
    *setBackground = 0;
    RGB color, reflexionColor, transparencyColor;
    LIST_NODE_PTR intersection = NULL;
    VECTOR vectorToEye_v, vectorReflexionOfV_R;
    long double productPointN_V;

    firstIntersection(anchor, rayFromEyeDirection, &intersection);

    if (intersection == NULL) { //If intersection is equals to NULL
        *setBackground = 1;
        color = backgroundColor;
    } else {

        //Calculate Vector V, which is a vector to points back to the light, it is the same vector D but with the inverse direction
        vectorToEye_v.x = rayFromEyeDirection->x * -1; //With this is inverse the direction
        vectorToEye_v.y = rayFromEyeDirection->y * -1;
        vectorToEye_v.z = rayFromEyeDirection->z * -1;

        iluminatePixel(anchor, rayFromEyeDirection, &color, &intersection->data.intersection);
        if (APPLY_MIRRORS) {
            if (intersection->data.intersection.object.O2 > 0.0 && reflexionLevel > 0) {

                int isReflexionBackground = 0;
                productPointN_V = dotProduct(&intersection->data.intersection.object.dependedNormal, &vectorToEye_v);
                vectorReflexionOfV_R.x = 2 * productPointN_V * intersection->data.intersection.object.dependedNormal.x - vectorToEye_v.x;
                vectorReflexionOfV_R.y = 2 * productPointN_V * intersection->data.intersection.object.dependedNormal.y - vectorToEye_v.y;
                vectorReflexionOfV_R.z = 2 * productPointN_V * intersection->data.intersection.object.dependedNormal.z - vectorToEye_v.z;
                reflexionColor = whichColor(&intersection->data.intersection.intersection, &vectorReflexionOfV_R, reflexionLevel - 1, &isReflexionBackground);


                if (isReflexionBackground == 0) {
                    color.r = intersection->data.intersection.object.O1 * color.r + intersection->data.intersection.object.O2 * reflexionColor.r;
                    color.g = intersection->data.intersection.object.O1 * color.g + intersection->data.intersection.object.O2 * reflexionColor.g;
                    color.b = intersection->data.intersection.object.O1 * color.b + intersection->data.intersection.object.O2 * reflexionColor.b;
                }


                /*
                                color.r = intersection->data.intersection.object.O1 * color.r + intersection->data.intersection.object.O2 * reflexionColor.r;
                                color.g = intersection->data.intersection.object.O1 * color.g + intersection->data.intersection.object.O2 * reflexionColor.g;
                                color.b = intersection->data.intersection.object.O1 * color.b + intersection->data.intersection.object.O2 * reflexionColor.b;
                 */

            }

        }

        if (APPLY_TRANSPARENCY) {
            if (intersection->data.intersection.object.O3 > 0.0 && intersection->nextPtr != NULL) {
                iluminatePixel(anchor, rayFromEyeDirection, &transparencyColor, &intersection->nextPtr->data.intersection);
                color.r += intersection->data.intersection.object.O3 * transparencyColor.r;
                color.g += intersection->data.intersection.object.O3 * transparencyColor.g;
                color.b += intersection->data.intersection.object.O3 * transparencyColor.b;
            }
        }





    }

    cleanList(&intersection);

    return color;
}

void iluminatePixel(VECTOR *anchor, VECTOR *rayFromEyeDirection, RGB *color, INTERSECTION *intersection) {

    int sumDiffuse = 0, sumEspecular = 0;
    LIST_NODE_PTR lights = lightsList;
    LIST_NODE_PTR obstacle = NULL;
    long double i = 0.0; //Light Intensity for diffuse illumination
    long double e = 0.0; //Intensity of specular reflexion
    long double productPointResult;
    long double productPointN_L;
    long double lightIntensityThrowObject = 1.0; //Controls the intensity of the light when it pass throw an object, if no object is on the way of the light the intensity is 1

    long double distanceToLight;
    VECTOR vectorToLight_l, vectorToEye_v, vectorReflexionOfL_r;

    //Test that the normal is the correct one
    if (dotProduct(rayFromEyeDirection, &intersection->object.normal) > EPSILON) {
        VECTOR newN;
        newN.x = -1 * intersection->object.normal.x;
        newN.y = -1 * intersection->object.normal.y;
        newN.z = -1 * intersection->object.normal.z;
        intersection->object.dependedNormal = newN;
    }

    while (lights != NULL) {
        LIGHT_POINT currentLight = lights->data.light;
        vectorToLight_l = calculateVectorToLight_L(intersection->intersection, currentLight.position, &distanceToLight);

        //Calculate Vector V, which is a vector to points back to the light, it is the same vector D but with the inverse direction
        vectorToEye_v.x = rayFromEyeDirection->x * -1; //With this is inverse the direction
        vectorToEye_v.y = rayFromEyeDirection->y * -1;
        vectorToEye_v.z = rayFromEyeDirection->z * -1;

        //Calculate Vector R, which is a vector ho is a Reflexion of the vector L taken the VEctor N as reference
        productPointN_L = dotProduct(&intersection->object.dependedNormal, &vectorToLight_l);
        vectorReflexionOfL_r.x = (2 * intersection->object.dependedNormal.x * productPointN_L) - vectorToLight_l.x;
        vectorReflexionOfL_r.y = (2 * intersection->object.dependedNormal.y * productPointN_L) - vectorToLight_l.y;
        vectorReflexionOfL_r.z = (2 * intersection->object.dependedNormal.z * productPointN_L) - vectorToLight_l.z;

        long double fAtt = attenuationFactor(intersection->intersection, currentLight);
        productPointResult = dotProduct(&vectorToLight_l, &intersection->object.dependedNormal);

        if (productPointResult > EPSILON) {

            if (APPLY_SHADOWS) {

                firstIntersection(&intersection->intersection, &vectorToLight_l, &obstacle);

                if (obstacle == NULL) {
                    sumDiffuse = 1;

                    if (APPLY_SPECULAR_REFLEXION) {
                        sumEspecular = 1;
                    }
                } else {
                    lightIntensityThrowObject = obstacle->data.intersection.object.translucencyCoefficient_kt;
                    sumDiffuse = 1;

                    if (APPLY_SPECULAR_REFLEXION) {
                        sumEspecular = 1;
                    }
                }

                //I have to clean the obstacle intersections list, because it depends on the light
                cleanList(&obstacle);
                obstacle = NULL;
            } else {
                sumDiffuse = 1;

                if (APPLY_SPECULAR_REFLEXION) {
                    sumEspecular = 1;
                }
            }

        }

        if (sumDiffuse == 1) {
            i += productPointResult * intersection->object.diffusionCoefficient_Kd * fAtt * (currentLight.intensity_Ip * lightIntensityThrowObject);
        }

        if (sumEspecular == 1) {
            long double dotProductResultRV = dotProduct(&vectorReflexionOfL_r, &vectorToEye_v);
            if (productPointN_L > EPSILON && dotProductResultRV > EPSILON) {
                e += pow(dotProductResultRV, intersection->object.specularReflexionRefinement_Kn) * intersection->object.specularReflexionCoeffient_Ks * (currentLight.intensity_Ip * lightIntensityThrowObject) * fAtt;
            }
        }
        sumDiffuse = 0;
        sumEspecular = 0;
        lights = lights->nextPtr;
        lightIntensityThrowObject = 1.0;
    }

    i += (intersection->object.ambientLightingCoefficient_Ka * ambientLighting_IA);

    if (i > 1.0) {
        i = 1.0;
    }

    color->r = intersection->object.color.r * i;
    color->g = intersection->object.color.g * i;
    color->b = intersection->object.color.b * i;

    if (APPLY_SPECULAR_REFLEXION) {
        color->r = color->r + e * (1.0 - color->r);
        color->g = color->g + e * (1.0 - color->g);
        color->b = color->b + e * (1.0 - color->b);
    }


}

void firstIntersection(VECTOR *eyePosition, VECTOR *rayFromEyeDirection, LIST_NODE_PTR *intersections) {
    LIST_NODE_PTR scene = sceneList;

    while (scene != NULL) {
        switch (scene->data.object.type) {
            case SPHERE:
                sphereIntersection(eyePosition, rayFromEyeDirection, &scene->data.object, intersections);
                break;
            case POLYGON:
                if (stop) {
                    int trash = 0;
                }
                polygonIntersection(eyePosition, rayFromEyeDirection, &scene->data.object, intersections);
                break;
            case DISK:
                diskIntersection(eyePosition, rayFromEyeDirection, &scene->data.object, intersections);
                break;
            case CYLINDER:
                cylinderInterseption(eyePosition, rayFromEyeDirection, &scene->data.object, intersections);
                break;
            case CONE:
                coneInterseption(eyePosition, rayFromEyeDirection, &scene->data.object, intersections);
                break;
        }
        scene = scene->nextPtr;
    }
}

/*
 * This method return a vector who points to the light point who is passed as parameter from a point
 */
VECTOR calculateVectorToLight_L(VECTOR intersection, VECTOR lightPoint, long double *distance) {
    *distance = calculateNormBetweenTwoVectors(&lightPoint, &intersection);
    VECTOR l;
    l.x = (lightPoint.x - intersection.x) / *distance;
    l.y = (lightPoint.y - intersection.y) / *distance;
    l.z = (lightPoint.z - intersection.z) / *distance;
    return l;
}

/*
 * When the light travels it starts losing energy, this method calculates the energy
 * that the light loses traveling to the object
 */
long double attenuationFactor(VECTOR intersection, LIGHT_POINT light) {
    long double distance = calculateNormBetweenTwoVectors(&intersection, &light.position);
    long double fAtt = 1 / (light.c1 + (light.c2 * distance) + (light.c3 * pow(distance, 2)));

    if (fAtt > 1.0) {
        return 1.0;
    } else {
        return fAtt;
    }
}

/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
   success, non-zero on error. */
int save_png_to_file(const char *path) {
    FILE *fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;


    /* "status" contains the return value of this function. At first
       it is set to a value which means 'failure'. When the routine
       has finished its work, it is set to a value which means
       'success'. */
    int status = -1;

    /* The following number is set by trial and error only. I cannot
       see where it it is documented in the libpng manual.
     */
    int pixel_size = 3;
    int depth = 8;


    /****************************************************************************/
    /****************************************************************************/
    /****************************************************************************/
    /*                para el manejo de errores                                 */
    fp = fopen(path, "wb");
    if (!fp) {
        return status;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {

        fclose(fp);
        return status;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {

        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return status;
    }

    /* Set up error handling. */
    if (setjmp(png_jmpbuf(png_ptr))) {

        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return status;
    }
    /****************************************************************************/
    /****************************************************************************/
    /****************************************************************************/


    /* Set image attributes. */
    png_set_IHDR(png_ptr,
            info_ptr,

            H_RES, //bitmap->width,
            V_RES, // bitmap->height,
            depth,
            PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */
    png_byte **row_pointers = NULL;
    row_pointers = png_malloc(png_ptr, V_RES * sizeof (png_byte *));

    //recorro todo el eje vertical(y) para ir obteniendo las filas
    for (y = 0; y < V_RES; y++) {

        //obtener el inverso de la fila
        int y_temp = (V_RES - y) - 1;

        png_byte *row = png_malloc(png_ptr, sizeof (uint8_t) * H_RES * pixel_size);
        row_pointers[y_temp] = row;

        //recorro todos los puntos
        for (x = 0; x < H_RES; x++) {
            //pixel_t * pixel = pixel_at(bitmap, x, y);
            *row++ = (int) (frameBuffer[x][y].r * 255);
            *row++ = (int) (frameBuffer[x][y].g * 255);
            *row++ = (int) (frameBuffer[x][y].b * 255);

        }
    }

    /* Write the image data to "fp". */
    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */
    status = 0;
    for (y = 0; y < V_RES; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
}
