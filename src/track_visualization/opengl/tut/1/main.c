#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>


#define FREEGLUT_STATIC 1

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>



int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Tutorial");

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed\n");
        return 1;
    }

    // OpenGL setup
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutMainLoop();

    return 0;
}
