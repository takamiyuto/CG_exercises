#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define N 10000



typedef struct {
    double x;
    double y;
} Point;


Point* generatePoints(){
    double vertices[3][2] = {
            {0, 0},
            {1., 0.},
            {0.5, sqrt(3)/2}
    };
    Point* points = (Point*)malloc(N*sizeof(Point));
    Point currentPoint = {0.5, 0.5};

    for (int i = 0; i < N; i++) {
        int vertexIndex = rand() % 3;
        currentPoint.x = (currentPoint.x + vertices[vertexIndex][0]) / 2;
        currentPoint.y = (currentPoint.y + vertices[vertexIndex][1]) / 2;
        points[i] = currentPoint;
    }

    return points;
}
static void display(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Repaint the background with the color
    // specified by glClearColor()
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5.f);

    // TODO
    //正三角形を定義
    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2d(0.,0);
    glVertex2d(1,0);
    glVertex2d(0.5, sqrt(3)/2);
    glEnd();
    //ランダムウォークの点を描画
    Point* points = (Point *) generatePoints();
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for(int i = 0; i < N; i++){
        glVertex2d(points[i].x, points[i].y);
    }
    glEnd();

    free(points);

    glutSwapBuffers();
}


static void initGL(void) {
    // Set the color for painting the background (White)
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Set an orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}


static void keyHandle(unsigned char key, int x, int y) {
    // Exit when ESC is pressed
    if (key == 27) exit(0);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Setup");

    initGL();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyHandle);
    glutMainLoop();
    return 0;
}