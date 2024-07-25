//
// Created by 高見優翔 on 24/07/22.
//
#ifdef __APPLE__

#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define _PI 3.14159f

static void makeOFF(void) {
    FILE *fp;
    fp = fopen("icosahedron.off", "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open file icosahedron.off\n");
        exit(1);
    }

    fprintf(fp, "OFF\n");
    fprintf(fp, "12 20 0\n");

    GLfloat phi = (1.f + sqrtf(5.f)) * .5f;
    GLfloat a = 1.f;
    GLfloat b = 1.f / phi;

    GLfloat vertices[12][3] = {{0.f, b,   -a},
                               {b,   a,   0.f},
                               {-b,  a,   0.f},
                               {0.f, b,   a},
                               {0.f, -b,  a},
                               {-a,  0.f, b},
                               {0.f, -b,  -a},
                               {a,   0.f, -b},
                               {a,   0.f, b},
                               {-a,  0.f, -b},
                               {b,   -a,  0.f},
                               {-b,  -a,  0.f}
    };

    int faces[20][3] = {{2,  1,  0},
                        {1,  2,  3},
                        {5,  4,  3},
                        {4,  8,  3},
                        {7,  6,  0},
                        {6,  9,  0},
                        {11, 10, 4},
                        {10, 11, 6},
                        {9,  5,  2},
                        {5,  9,  11},
                        {8,  7,  1},
                        {7,  8,  10},
                        {2,  5,  3},
                        {8,  1,  3},
                        {9,  2,  0},
                        {1,  7,  0},
                        {11, 9,  6},
                        {7,  10, 6},
                        {5,  11, 4},
                        {10, 8,  4}
    };

    int i;
    for (i = 0; i < 12; i++) {
        fprintf(fp, "%f %f %f\n", vertices[i][0], vertices[i][1], vertices[i][2]);
    }

    for (i = 0; i < 20; i++) {
        fprintf(fp, "3 %d %d %d\n", faces[i][0], faces[i][1], faces[i][2]);
    }

    fclose(fp);
}



int main(int argc, char **argv) {
    makeOFF();
    return 0;
}