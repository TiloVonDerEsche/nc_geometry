#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define FREEGLUT_STATIC 1

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>

// parse_line and trim
#include "helper_funcs.h"

#define SPEED 0.3f
#define SHIFT_BOOST 0.5f

// Structure to hold track data
typedef struct {
    float ax, ay, az; // Start point
    float bx, by, bz; // End point
    float hradius, vradius; // Radii
} Track;

// Camera variables
float camX = 0.0f, camY = 0.0f, camZ = 50.0f;
float camRoll = 0.0f, camPitch = 0.0f, camYaw = 0.0f;
int lastX = 0, lastY = 0;

int buttonDown = 0;
int isShiftPressed = 0;
int keys[256] = {0};

// Track data
Track* tracks = NULL;
int numTracks = 0;
float min_z, max_z;  // Global variables for z-range color mapping

Config config = {0};

// Ambient Light Control Variable
float ambientIntensity = 0.2f; // Default 20% ambient light

int read_config(const char* filename, Config* config) {
    printf("Opening %s...\n", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open config file %s\n", filename);
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);

        // ignore comments and empty lines
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
        } else if (strcmp(key, "horizontal_radius") == 0) {
            config->horizontal_radius = atof(value);
        } else if (strcmp(key, "vertical_radius") == 0) {
            config->vertical_radius = atof(value);
        }
    }

    fclose(file);
    return 0;
}

// Read CSV file
void read_csv(const char* filename) {
    printf("Opening %s...\n", filename);
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
        sscanf(line, "%*d,%f,%f,%f, %f,%f,%f",
               &tracks[i].ax, &tracks[i].ay, &tracks[i].az,
               &tracks[i].bx, &tracks[i].by, &tracks[i].bz);
        tracks[i].hradius = config.horizontal_radius;
        tracks[i].vradius = config.vertical_radius;
        i++;
    }
    fclose(file);
}

void drawCylinder(float ax, float ay, float az, float bx, float by, float bz, float hradius, float vradius) {
    const int segments = 32;

    float dx = bx - ax;
    float dy = by - ay;
    float dz = bz - az;
    float length = sqrt(dx * dx + dy * dy + dz * dz);
    if (length == 0) return;
    dx /= length;
    dy /= length;
    dz /= length;

    float up[3] = {0.0f, 0.0f, 1.0f};

    float dot_up_w = up[2] * dz;
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
        v[0] = 1.0f; v[1] = 0.0f; v[2] = 0.0f;
    }

    float u[3];
    u[0] = dy * v[2] - dz * v[1];
    u[1] = dz * v[0] - dx * v[2];
    u[2] = dx * v[1] - dy * v[0];

    GLfloat rotationMatrix[16] = {
        u[0], u[1], u[2], 0.0f,
        v[0], v[1], v[2], 0.0f,
        dx, dy, dz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

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

    // Dynamically update ambient light intensity
    GLfloat ambientLight[] = {ambientIntensity, ambientIntensity, ambientIntensity, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    // Set directional light position
    GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Camera setup
    glRotatef(camPitch, 1, 0, 0);
    glRotatef(camYaw, 0, 1, 0);
    glTranslatef(-camX, -camY, -camZ);

    // Draw tracks
    for (int i = 0; i < numTracks; i++) {
        float z_avg = (tracks[i].az + tracks[i].bz) / 2.0f;
        float t = (max_z - min_z) > 0 ? (z_avg - min_z) / (max_z - min_z) : 0.5f;
        float r, g, b;

        if (t < 0.25f) {
            r = 0.0f; g = 4.0f * t; b = 1.0f;
        } else if (t < 0.5f) {
            r = 0.0f; g = 1.0f; b = 1.0f - 4.0f * (t - 0.25f);
        } else if (t < 0.75f) {
            r = 4.0f * (t - 0.5f); g = 1.0f; b = 0.0f;
        } else {
            r = 1.0f; g = 1.0f - 4.0f * (t - 0.75f); b = 0.0f;
        }

        glColor3f(r, g, b);
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

void keyDown(unsigned char key, int x, int y) {
    keys[tolower(key)] = 1;

    // Controls for Ambient Light Intensity (+ / -)
    if (key == '+' || key == '=') {
        ambientIntensity += 0.05f;
        if (ambientIntensity > 1.0f) ambientIntensity = 1.0f;
        printf("Ambient Light Intensity: %.2f\n", ambientIntensity);
    } else if (key == '-' || key == '_') {
        ambientIntensity -= 0.05f;
        if (ambientIntensity < 0.0f) ambientIntensity = 0.0f;
        printf("Ambient Light Intensity: %.2f\n", ambientIntensity);
    }
}

void keyUp(unsigned char key, int x, int y) {
    keys[tolower(key)] = 0;
}

void modeKeyDown(int key, int x, int y) {
    if (key == GLUT_KEY_SHIFT_L || key == GLUT_KEY_SHIFT_R) {
        isShiftPressed = 1;
    }
}

void modeKeyUp(int key, int x, int y) {
    if (key == GLUT_KEY_SHIFT_L || key == GLUT_KEY_SHIFT_R) {
        isShiftPressed = 0;
    }
}

void handle_movement(int garbage) {
    float speed = SPEED;
    if (isShiftPressed) speed = SPEED + SHIFT_BOOST;

    float yawRad = camYaw * M_PI / 180.0f;
    float forwardX = sinf(yawRad);
    float forwardZ = -cosf(yawRad);
    float rightX = cosf(yawRad);
    float rightZ = sinf(yawRad);

    if (keys['w']) { camX += forwardX * speed; camZ += forwardZ * speed; }
    if (keys['s']) { camX -= forwardX * speed; camZ -= forwardZ * speed; }
    if (keys['a']) { camX -= rightX * speed; camZ -= rightZ * speed; }
    if (keys['d']) { camX += rightX * speed; camZ += rightZ * speed; }
    if (keys[' ']) { camY += speed; }
    if (keys['c']) { camY -= speed; }

    glutPostRedisplay();
    glutTimerFunc(16, handle_movement, 0);
}

int main(int argc, char** argv) {
    read_config("config.txt", &config);
    read_csv(config.tracks_to_plot);

    min_z = tracks[0].az;
    max_z = tracks[0].az;
    for (int i = 0; i < numTracks; i++) {
        if (tracks[i].az < min_z) min_z = tracks[i].az;
        if (tracks[i].az > max_z) max_z = tracks[i].az;
        if (tracks[i].bz < min_z) min_z = tracks[i].bz;
        if (tracks[i].bz > max_z) max_z = tracks[i].bz;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Track Visualizer");

    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed\n");
        return 1;
    }

    // OpenGL Setup
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE); // Rescales normal vectors after glScalef
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Enable Lighting Pipeline
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Light Colors
    GLfloat diffuseLight[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Enable Material Color Mapping (binds glColor3f to Ambient and Diffuse)
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Material Specular Properties
    GLfloat specularMaterial[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[]        = {50.0f}; // Range: 0.0 to 128.0
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // GLUT Callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(modeKeyDown);
    glutSpecialUpFunc(modeKeyUp);
    glutTimerFunc(0, handle_movement, 0);

    glutMainLoop();

    free(tracks);
    return 0;
}
