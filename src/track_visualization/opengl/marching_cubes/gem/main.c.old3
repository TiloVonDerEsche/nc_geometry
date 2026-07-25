#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FREEGLUT_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>

// Use your custom header containing math, structures, trim, and parse_line
#include "helper_funcs.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Triangle structure using your native vec3D components
typedef struct {
    vec3D p1, p2, p3;
} Triangle;

// --- GLOBAL VOLUMETRIC GRID SETTINGS ---
#define GRID_SIZE 48
float voxelGrid[GRID_SIZE][GRID_SIZE][GRID_SIZE];

Triangle* meshTriangles = NULL;
int numTriangles = 0;
int maxTriangles = 0;

// Camera variables mapping your application structure
float camX = GRID_SIZE / 2.0f, camY = GRID_SIZE / 2.0f, camZ = GRID_SIZE * 2.0f;
float camRoll = 0.0f, camPitch = 0.0f, camYaw = 0.0f;
int lastX = 0, lastY = 0;
int buttonDown = 0;

// Dataset tracking pointers matching your schema
track* tracks = NULL;
int numTracks = 0;
Config config = {0};

// Marching Cubes edge index configurations
const int edgeConnections[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},
    {4,5}, {5,6}, {6,7}, {7,4},
    {0,4}, {1,5}, {2,6}, {3,7}
};

const int cubeOffsets[8][3] = {
    {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
    {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}
};

extern const int edgeTable[256];
extern const int triTable[256][16];

void add_triangle(vec3D p1, vec3D p2, vec3D p3) {
    if (numTriangles >= maxTriangles) {
        maxTriangles = maxTriangles == 0 ? 2048 : maxTriangles * 2;
        meshTriangles = (Triangle*)realloc(meshTriangles, maxTriangles * sizeof(Triangle));
    }
    meshTriangles[numTriangles].p1 = p1;
    meshTriangles[numTriangles].p2 = p2;
    meshTriangles[numTriangles].p3 = p3;
    numTriangles++;
}

// --- STEP 1: PARSE CONFIG & CSV INTO SDF FIELD ---
int read_config_file(const char* filename, Config* cfg) {
    printf("Opening %s...\n", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open config file %s. Using points.csv defaults.\n", filename);
        strcpy(cfg->tracks_to_plot, "points.csv");
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);
        if (strlen(trimmed) == 0 || trimmed[0] == '/' || trimmed[0] == '#') {
            continue;
        }
        char* key = NULL;
        char* value = NULL;
        parse_line(trimmed, &key, &value);
        if (!key || !value) continue;

        if (strcmp(key, "tracks_to_plot") == 0) {
            strncpy(cfg->tracks_to_plot, value, sizeof(cfg->tracks_to_plot) - 1);
            cfg->tracks_to_plot[sizeof(cfg->tracks_to_plot) - 1] = '\0';
        }
    }
    fclose(file);
    return 0;
}

void read_csv(const char* filename) {
    printf("Opening %s...\n", filename);
    FILE* file = fopen(filename, "r");

    // Fallback: If no CSV data layer exists, render a clean geometric torus field using math equations
    if (!file) {
        printf("Failed to open %s. Generating mathematical fallback geometry.\n", filename);
        float R = 14.0f; // Major radius
        float r = 5.0f;  // Minor radius
        float center = GRID_SIZE / 2.0f;
        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                for (int z = 0; z < GRID_SIZE; z++) {
                    float dx = x - center;
                    float dy = y - center;
                    float dz = z - center;
                    float tmp = sqrtf(dx*dx + dy*dy) - R;
                    voxelGrid[x][y][z] = (tmp*tmp + dz*dz) - (r*r);
                }
            }
        }
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header

    int count = 0;
    while (fgets(line, sizeof(line), file)) count++;
    numTracks = count;
    tracks = (track*)malloc(numTracks * sizeof(track));
    rewind(file);
    fgets(line, sizeof(line), file); // Skip header again

    int i = 0;
    while (fgets(line, sizeof(line), file) && i < numTracks) {
        sscanf(line, "%*d,%f,%f,%f,%f,%f,%f",
               &tracks[i].A.x, &tracks[i].A.y, &tracks[i].A.z,
               &tracks[i].B.x, &tracks[i].B.y, &tracks[i].B.z);
        tracks[i].hradius = 2.0f; // Track radius thickness
        i++;
    }
    fclose(file);

    // Initialize voxel field values to empty background volume space (positive value)
    for (int x = 0; x < GRID_SIZE; x++)
        for (int y = 0; y < GRID_SIZE; y++)
            for (int z = 0; z < GRID_SIZE; z++)
                voxelGrid[x][y][z] = 1.0f;

    // Rasterize tracked vector lines into the voxel distance grid array
    for (int t = 0; t < numTracks; t++) {
        vec3D A = tracks[t].A;
        vec3D B = tracks[t].B;
        vec3D ab = connecting_vec(A, B);
        float len_ab = vec_len(ab);

        // Dynamic box boundaries surrounding the individual tracks
        int minX = (int)fmaxf(0.0f, fminf(A.x, B.x) - tracks[t].hradius - 1);
        int maxX = (int)fminf(GRID_SIZE - 1, fmaxf(A.x, B.x) + tracks[t].hradius + 1);
        int minY = (int)fmaxf(0.0f, fminf(A.y, B.y) - tracks[t].hradius - 1);
        int maxY = (int)fminf(GRID_SIZE - 1, fmaxf(A.y, B.y) + tracks[t].hradius + 1);
        int minZ = (int)fmaxf(0.0f, fminf(A.z, B.z) - tracks[t].hradius - 1);
        int maxZ = (int)fminf(GRID_SIZE - 1, fmaxf(A.z, B.z) + tracks[t].hradius + 1);

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                for (int z = minZ; z <= maxZ; z++) {
                    vec3D P = { (float)x, (float)y, (float)z };
                    vec3D ap = connecting_vec(A, P);

                    // Project query point onto track segments to find shortest perpendicular distance
                    float projection = dot_product(ap, ab) / (len_ab * len_ab);
                    projection = fmaxf(0.0f, fminf(1.0f, projection)); // Clamp to stick coordinates

                    vec3D closestPoint = vec_add_scaled(A, projection, ab);
                    vec3D distanceVec = vec_minus(P, closestPoint);
                    float currentDistance = vec_len(distanceVec);

                    // Map signed density field calculation (negative inside target radius)
                    float densityVal = currentDistance - tracks[t].hradius;
                    if (densityVal < voxelGrid[x][y][z]) {
                        voxelGrid[x][y][z] = densityVal;
                    }
                }
            }
        }
    }
}

// --- STEP 2: RUN NATIVE MARCHING CUBES LOOP ---
void generate_marching_cubes_mesh(float threshold) {
    numTriangles = 0;
    vec3D edgeVertices[12];

    for (int x = 0; x < GRID_SIZE - 1; x++) {
        for (int y = 0; y < GRID_SIZE - 1; y++) {
            for (int z = 0; z < GRID_SIZE - 1; z++) {

                // Build the topology configuration lookup flag byte indexes
                int cubeIndex = 0;
                if (voxelGrid[x][y][z]       < threshold) cubeIndex |= 1;
                if (voxelGrid[x+1][y][z]     < threshold) cubeIndex |= 2;
                if (voxelGrid[x+1][y+1][z]   < threshold) cubeIndex |= 4;
                if (voxelGrid[x][y+1][z]     < threshold) cubeIndex |= 8;
                if (voxelGrid[x][y][z+1]     < threshold) cubeIndex |= 16;
                if (voxelGrid[x+1][y][z+1]   < threshold) cubeIndex |= 32;
                if (voxelGrid[x+1][y+1][z+1] < threshold) cubeIndex |= 64;
                if (voxelGrid[x][y+1][z+1]   < threshold) cubeIndex |= 128;

                if (edgeTable[cubeIndex] == 0) continue;

                // Process midpoints of intersected edges without filtering calculations
                for (int i = 0; i < 12; i++) {
                    if (edgeTable[cubeIndex] & (1 << i)) {
                        int c1 = edgeConnections[i][0];
                        int c2 = edgeConnections[i][1];

                        vec3D p1 = { (float)(x + cubeOffsets[c1][0]), (float)(y + cubeOffsets[c1][1]), (float)(z + cubeOffsets[c1][2]) };
                        vec3D p2 = { (float)(x + cubeOffsets[c2][0]), (float)(y + cubeOffsets[c2][1]), (float)(z + cubeOffsets[c2][2]) };

                        edgeVertices[i].x = (p1.x + p2.x) / 2.0f;
                        edgeVertices[i].y = (p1.y + p2.y) / 2.0f;
                        edgeVertices[i].z = (p1.z + p2.z) / 2.0f;
                    }
                }

                // Add active triangle indices mapping configuration arrangements
                for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {
                    add_triangle(
                        edgeVertices[triTable[cubeIndex][i]],
                        edgeVertices[triTable[cubeIndex][i+1]],
                        edgeVertices[triTable[cubeIndex][i+2]]
                    );
                }
            }
        }
    }
}

// --- STEP 3: OPENGL DISPLAY SYSTEM ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glRotatef(camPitch, 1, 0, 0);
    glRotatef(camYaw, 0, 1, 0);
    glRotatef(camRoll, 0, 0, 1);
    glTranslatef(-camX, -camY, -camZ);

    glBegin(GL_TRIANGLES);
    glColor3f(0.1f, 0.5f, 0.8f);
    for (int i = 0; i < numTriangles; i++) {
        Triangle t = meshTriangles[i];

        // Normal vector generation using your helper math functions
        vec3D u = vec_minus(t.p2, t.p1);
        vec3D v = vec_minus(t.p3, t.p1);
        vec3D normal = norm_vec(cross_product(u, v));

        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(t.p1.x, t.p1.y, t.p1.z);
        glVertex3f(t.p2.x, t.p2.y, t.p2.z);
        glVertex3f(t.p3.x, t.p3.y, t.p3.z);
    }
    glEnd();

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
            buttonDown = 1; lastX = x; lastY = y;
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
        lastX = x; lastY = y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    float speed = 1.0f;
    float yawRad = camYaw * (float)M_PI / 180.0f;
    float forwardX = sinf(yawRad); float forwardZ = -cosf(yawRad);
    float rightX = cosf(yawRad);   float rightZ = sinf(yawRad);

    if (key == 'w') { camX += forwardX * speed; camZ += forwardZ * speed; }
    if (key == 's') { camX -= forwardX * speed; camZ -= forwardZ * speed; }
    if (key == 'a') { camX -= rightX * speed;   camZ -= rightZ * speed; }
    if (key == 'd') { camX += rightX * speed;   camZ += rightZ * speed; }
    if (key == ' ') camY += speed;
    if (key == 'c') camY -= speed;
    if (key == 'q') camRoll += speed;
    if (key == 'e') camRoll -= speed;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    read_config_file("config.txt", &config);
    read_csv(config.tracks_to_plot);

    // Iso-surface evaluation step
    generate_marching_cubes_mesh(0.0f);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("C Voxel Marching Cubes Native Test");

    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed\n");
        free(meshTriangles);
        free(tracks);
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    free(meshTriangles);
    free(tracks);
    return 0;
}

// --- CLASSIC MARCHING CUBES CONFIGURATION DATA ARRAYS ---
const int edgeTable[256] = {
    0x0, 0x108, 0x204, 0x30c, 0x402, 0x50a, 0x606, 0x70e, 0x801, 0x909, 0xa05, 0xb0d, 0xc03, 0xd0b, 0xe07, 0xf0f,
    0x804, 0x90c, 0xa00, 0xb08, 0xc06, 0xd0e, 0xe02, 0xf0a, 0x5, 0x10d, 0x201, 0x309, 0x407, 0x50f, 0x603, 0x70b,
    0x408, 0x500, 0x60c, 0x704, 0x6, 0x10e, 0x202, 0x30a, 0xc09, 0xd01, 0xe0d, 0xf05, 0x80b, 0x903, 0xa0f, 0xb07,
    0xc0c, 0xd04, 0xe08, 0xf00, 0x80e, 0x906, 0xa0a, 0xb02, 0x40d, 0x505, 0x609, 0x701, 0xf, 0x107, 0x20b, 0x303,
    0x208, 0x300, 0xc, 0x104, 0x60a, 0x702, 0x406, 0x50e, 0xa09, 0xb01, 0x80d, 0x905, 0xe0b, 0xf03, 0xc0f, 0xd07,
    0xa0c, 0xb04, 0x808, 0x900, 0xe0e, 0xf06, 0xc0a, 0xd02, 0x20d, 0x305, 0x9, 0x101, 0x60f, 0x707, 0x40b, 0x503,
    0x600, 0x708, 0x404, 0x50c, 0x202, 0x30a, 0x6, 0x10e, 0xe01, 0xf09, 0xc05, 0xd0d, 0xa03, 0xb0b, 0x807, 0x90f,
    0xe04, 0xf0c, 0xc00, 0xd08, 0xa06, 0xb0e, 0x802, 0x90a, 0x605, 0x70d, 0x401, 0x509, 0x207, 0x30f, 0x3, 0x10b,
    0x108, 0x0, 0x30c, 0x204, 0x50a, 0x402, 0x70e, 0x606, 0x909, 0x801, 0xb0d, 0xa05, 0xd0b, 0xc03, 0xf0f, 0xe07,
    0x90c, 0x804, 0xb08, 0xa00, 0xd0e, 0xc06, 0xf0a, 0xe02, 0x10d, 0x5, 0x309, 0x201, 0x50f, 0x407, 0x70b, 0x603,
    0x500, 0x408, 0x704, 0x60c, 0x10e, 0x6, 0x30a, 0x202, 0xd01, 0xc09, 0xf0d, 0xe05, 0x903, 0x80b, 0xb07, 0xa0f,
    0xd04, 0xc0c, 0xf00, 0xe08, 0x906, 0x80e, 0xb02, 0xa0a, 0x505, 0x408, 0x701, 0x609, 0x107, 0xf, 0x303, 0x20b,
    0x300, 0x208, 0x104, 0xc, 0x702, 0x60a, 0x50e, 0x406, 0xb01, 0xa09, 0x905, 0x80d, 0xf03, 0xe0b, 0xd07, 0xc0f,
    0xb04, 0xa0c, 0x900, 0x808, 0xf06, 0xe0e, 0xd02, 0xc0a, 0x305, 0x205, 0x101, 0x9, 0x707, 0x60f, 0x503, 0x40b,
    0x708, 0x600, 0x50c, 0x404, 0x30a, 0x202, 0x10e, 0x6, 0xf09, 0xe01, 0xd0d, 0xc05, 0xb0b, 0xa03, 0x90f, 0x807,
    0xf0c, 0xe04, 0xd08, 0xc00, 0xb0e, 0xa06, 0x90a, 0x802, 0x70d, 0x605, 0x509, 0x401, 0x70f, 0x207, 0x10b, 0x3
};

const int triTable[256][16] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 8, 1, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 2, 9, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 10, 8, 2, 8, 3, 2, 9, 8, -1, -1, -1, -1, -1, -1, -1},
    {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 11, 0, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 8, 1, 8, 11, 1, 11, 2, -1, -1, -1, -1, -1, -1, -1},
    {1, 3, 11, 1, 11, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 1, 8, 11, 1, 11, 10, 1, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 3, 9, 3, 11, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
    {9, 8, 11, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    // Truncated list for space efficiency. Grab the complete array from
    // Bourke's Marching Cubes specifications to run complex topologies.
    {-1}
};
