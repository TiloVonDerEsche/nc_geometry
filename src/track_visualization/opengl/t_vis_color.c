#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define FREEGLUT_STATIC 1

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>

//parse_line and trim
#include "helper_funcs.h"

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

Config config = {0};

// Read CSV file
void read_csv(const char* filename) {
    printf("Opening %s...\n",filename);
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
        sscanf(line, "%*d,%f,%f,%f, %f,%f,%f",
               &tracks[i].ax, &tracks[i].ay, &tracks[i].az,
               &tracks[i].bx, &tracks[i].by, &tracks[i].bz);
        tracks[i].hradius = 0.5;
        tracks[i].vradius = 0.5;
        i++;
    }
    fclose(file);
}



int read_config(const char* filename, Config* config) {
    printf("Opening %s...\n",filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open config file %s\n", filename);
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);

        //ignore comments and empty lines
        if (strlen(trimmed) == 0 || trimmed[0] == '/' || trimmed[0] == '#') {
            continue;
        }
        char* key;
        char* value;
        parse_line(trimmed, &key, &value);
        if (!key || !value) {
            fprintf(stderr, "Error: Invalid line format: %s\n", line);
            continue;
        }


        if (strcmp(key, "tracks_to_plot") == 0) {
            strncpy(config->tracks_to_plot, value, sizeof(config->tracks_to_plot) - 1);
            config->tracks_to_plot[sizeof(config->tracks_to_plot) - 1] = '\0';
        }
    }

    fclose(file);
    return 0;
}

// Draw an elliptical cylinder from point A to B with correct orientation
void drawCylinder(float ax, float ay, float az, float bx, float by, float bz, float hradius, float vradius) {
    const int segments = 32;

    // Compute direction vector and length
    float dx = bx - ax;
    float dy = by - ay;
    float dz = bz - az;
    float length = sqrt(dx * dx + dy * dy + dz * dz);
    if (length == 0) return;
    dx /= length;
    dy /= length;
    dz /= length;

    float up[3] = {0.0f, 0.0f, 1.0f};
    // Check if direction is nearly parallel to y-axis
    // if (fabs(fabs(dy) - 1.0f) < 0.001f) {
    //     up[0] = 0.0f; up[1] = 0.0f; up[2] = 1.0f; // Use z-axis as up
    // }

    // Compute local y-axis: v = up - (up • w) * w
    float dot_up_w = up[2] * dz; //up[0] * dx + up[1] * dy +
    float v[3];
    v[0] = up[0] - dot_up_w * dx;
    v[1] = up[1] - dot_up_w * dy;
    v[2] = up[2] - dot_up_w * dz;
    float v_len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (v_len > 0) {
        v[0] /= v_len;
        v[1] /= v_len;
        v[2] /= v_len;
    } else {
        v[0] = 1.0f; v[1] = 0.0f; v[2] = 0.0f; // Fallback
    }

    // Compute local x-axis: u = w × v
    float u[3];
    u[0] = dy * v[2] - dz * v[1];
    u[1] = dz * v[0] - dx * v[2];
    u[2] = dx * v[1] - dy * v[0];

    // Construct rotation matrix (column-major for OpenGL)
    GLfloat rotationMatrix[16] = {
        u[0], u[1], u[2], 0.0f,
        v[0], v[1], v[2], 0.0f,
        dx, dy, dz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Apply transformations
    glPushMatrix();
    glTranslatef(ax, ay, az);
    glMultMatrixf(rotationMatrix);
    glScalef(hradius, vradius, length);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
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
    read_config("config.txt",&config);
    read_csv(config.tracks_to_plot);

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

    // Set material properties for specular highlights
    //GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    //GLfloat shininess[] = {50.0f};
    //glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

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
