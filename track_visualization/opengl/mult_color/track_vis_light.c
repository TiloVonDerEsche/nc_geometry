#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>  // Added for GLU functions

// Structure to hold track data
typedef struct {
    float ax, ay, az; // Start point
    float bx, by, bz; // End point
    float hradius, vradius; // Radii
} Track;

// Camera variables
float camX = 0.0f, camY = 0.0f, camZ = 50.0f;
float camYaw = 0.0f, camPitch = 0.0f;
int lastX = 0, lastY = 0;
int buttonDown = 0;

// Track data
Track* tracks = NULL;
int numTracks = 0;
float min_z, max_z;  // Global variables for z-range color mapping

// Read CSV file
void readCSV(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open %s\n", filename);
        exit(1);
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header

    int count = 0;
    while (fgets(line, sizeof(line), file)) count++;
    numTracks = count;
    tracks = (Track*)malloc(numTracks * sizeof(Track));
    rewind(file);
    fgets(line, sizeof(line), file); // Skip header again

    int i = 0;
    while (fgets(line, sizeof(line), file) && i < numTracks) {
        float dummy;
        sscanf(line, "%*d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
               &tracks[i].ax, &tracks[i].ay, &tracks[i].az,
               &tracks[i].bx, &tracks[i].by, &tracks[i].bz,
               &tracks[i].hradius, &tracks[i].vradius,
               &dummy, &dummy, &dummy);
        i++;
    }
    fclose(file);
}

// Draw an elliptical cylinder from point A to B
void drawCylinder(float ax, float ay, float az, float bx, float by, float bz, float hradius, float vradius) {
    const int segments = 32;
    float length = sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay) + (bz - az) * (bz - az));
    if (length == 0) return;

    float dx = (bx - ax) / length;
    float dy = (by - ay) / length;
    float dz = (bz - az) / length;

    float up[3] = {0, 0, 1};
    float axis[3];
    float dot = dz;
    float angle = acos(dot) * 180.0f / M_PI;

    if (fabs(dot) < 0.999f) {
        axis[0] = dy;
        axis[1] = -dx;
        axis[2] = 0;
        float axisLen = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
        if (axisLen > 0) {
            axis[0] /= axisLen;
            axis[1] /= axisLen;
        }
    } else {
        axis[0] = 1; axis[1] = 0; axis[2] = 0;
        if (dot < 0) angle = 180.0f;
    }

    glPushMatrix();
    glTranslatef(ax, ay, az);
    if (fabs(dot) < 0.999f) {
        glRotatef(angle, axis[0], axis[1], axis[2]);
    } else if (dot < 0) {
        glRotatef(180.0f, 1, 0, 0);
    }

    glScalef(hradius, vradius, length);

    // Draw cylinder with normals for lighting
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);  // Enable smooth normals
    gluCylinder(quad, 1.0, 1.0, 1.0, segments, 1);
    gluDeleteQuadric(quad);

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set light position in world coordinates
    GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};  // Directional light from top-right-front
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Camera setup
    glTranslatef(0, 0, -camZ);
    glRotatef(camPitch, 1, 0, 0);
    glRotatef(camYaw, 0, 1, 0);
    glTranslatef(-camX, -camY, 0);

    // Draw tracks with color based on z-position
    for (int i = 0; i < numTracks; i++) {
        float z_avg = (tracks[i].az + tracks[i].bz) / 2.0f;
        float t = (max_z - min_z) > 0 ? (z_avg - min_z) / (max_z - min_z) : 0.5f;  // Handle zero range
        float r, g, b;
        // Jet-like color map: blue -> green -> red
        if (t < 0.25f) {
            r = 0.0f;
            g = 4.0f * t;
            b = 1.0f;
        } else if (t < 0.5f) {
            r = 0.0f;
            g = 1.0f;
            b = 1.0f - 4.0f * (t - 0.25f);
        } else if (t < 0.75f) {
            r = 4.0f * (t - 0.5f);
            g = 1.0f;
            b = 0.0f;
        } else {
            r = 1.0f;
            g = 1.0f - 4.0f * (t - 0.75f);
            b = 0.0f;
        }
        glColor3f(r, g, b);  // Set color per track
        drawCylinder(tracks[i].ax, tracks[i].ay, tracks[i].az,
                     tracks[i].bx, tracks[i].by, tracks[i].bz,
                     tracks[i].hradius, tracks[i].vradius);
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            buttonDown = 1;
            lastX = x;
            lastY = y;
        } else {
            buttonDown = 0;
        }
    }
}

void motion(int x, int y) {
    if (buttonDown) {
        camYaw += (x - lastX) * 0.1f;
        camPitch += (y - lastY) * 0.1f;
        if (camPitch > 89.0f) camPitch = 89.0f;
        if (camPitch < -89.0f) camPitch = -89.0f;
        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    float speed = 1.0f;
    if (key == 'w') camZ -= speed;
    if (key == 's') camZ += speed;
    if (key == 'a') camX -= speed;
    if (key == 'd') camX += speed;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Read CSV
    readCSV("track_list.csv");

    // Compute min and max z for color mapping
    min_z = tracks[0].az;
    max_z = tracks[0].az;
    for (int i = 0; i < numTracks; i++) {
        if (tracks[i].az < min_z) min_z = tracks[i].az;
        if (tracks[i].az > max_z) max_z = tracks[i].az;
        if (tracks[i].bz < min_z) min_z = tracks[i].bz;
        if (tracks[i].bz > max_z) max_z = tracks[i].bz;
    }

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Track Visualizer");

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed\n");
        return 1;
    }

    // OpenGL setup
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  // glColor3f affects ambient and diffuse

    // Set global ambient light
    GLfloat ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // Set material properties for specular highlights
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[] = {50.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    free(tracks);
    return 0;
}
