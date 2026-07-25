#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FREEGLUT_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>

#include "helper_funcs.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Raw point format from ncc_points.csv
typedef struct {
    int track_id;
    int point_id;
    vec3D pos;
    float laser_power;
    float machine_speed;
} NCCPoint;

typedef struct {
    vec3D p1, p2, p3;
} Triangle;

// --- GLOBAL VOLUMETRIC GRID SETTINGS ---
#define GRID_SIZE 64
float voxelGrid[GRID_SIZE][GRID_SIZE][GRID_SIZE];

Triangle* meshTriangles = NULL;
int numTriangles = 0;
int maxTriangles = 0;

// Camera variables
float camX = 0.0f, camY = 0.0f, camZ = 300.0f;
float camRoll = 0.0f, camPitch = 0.0f, camYaw = 0.0f;
int lastX = 0, lastY = 0;
int buttonDown = 0;

// Marching Cubes lookup connections
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

// --- STEP 1: PARSE NCC_POINTS.CSV & RASTERIZE SDF ---
void read_ncc_csv(const char* filename) {
    printf("Opening %s...\n", filename);
    FILE* file = fopen(filename, "r");

    if (!file) {
        printf("Error: Could not open %s. Generating fallback torus.\n", filename);
        float R = 18.0f, r = 6.0f, center = GRID_SIZE / 2.0f;
        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                for (int z = 0; z < GRID_SIZE; z++) {
                    float dx = x - center, dy = y - center, dz = z - center;
                    float tmp = sqrtf(dx*dx + dy*dy) - R;
                    voxelGrid[x][y][z] = (tmp*tmp + dz*dz) - (r*r);
                }
            }
        }
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip CSV header

    // Count line items
    int capacity = 1024;
    int pointCount = 0;
    NCCPoint* rawPoints = (NCCPoint*)malloc(capacity * sizeof(NCCPoint));

    while (fgets(line, sizeof(line), file)) {
        if (pointCount >= capacity) {
            capacity *= 2;
            rawPoints = (NCCPoint*)realloc(rawPoints, capacity * sizeof(NCCPoint));
        }

        NCCPoint pt;
        int parsed = sscanf(line, "%d, %d, %f, %f, %f, %f, %f",
                           &pt.track_id, &pt.point_id,
                           &pt.pos.x, &pt.pos.y, &pt.pos.z,
                           &pt.laser_power, &pt.machine_speed);

        if (parsed == 7) {
            rawPoints[pointCount++] = pt;
        }
    }
    fclose(file);
    printf("Loaded %d coordinates from %s.\n", pointCount, filename);

    // Initialize voxel field to positive distance background (empty)
    for (int x = 0; x < GRID_SIZE; x++)
        for (int y = 0; y < GRID_SIZE; y++)
            for (int z = 0; z < GRID_SIZE; z++)
                voxelGrid[x][y][z] = 1000.0f;

    float trackRadius = 1.5f; // Meltpool radius thickness

    // Rasterize point segments sharing identical Track_id
    for (int i = 0; i < pointCount - 1; i++) {
        NCCPoint pA = rawPoints[i];
        NCCPoint pB = rawPoints[i + 1];

        // Segment condition: Same track ID and active laser power
        if (pA.track_id != pB.track_id || pA.laser_power <= 0.0f) {
            continue;
        }

        vec3D A = pA.pos;
        vec3D B = pB.pos;
        vec3D ab = connecting_vec(A, B);
        float len_ab = vec_len(ab);
        if (len_ab < 0.0001f) continue;

        // Dynamic local boundary limits
        int minX = (int)fmaxf(0.0f, fminf(A.x, B.x) - trackRadius - 1);
        int maxX = (int)fminf(GRID_SIZE - 1, fmaxf(A.x, B.x) + trackRadius + 1);
        int minY = (int)fmaxf(0.0f, fminf(A.y, B.y) - trackRadius - 1);
        int maxY = (int)fminf(GRID_SIZE - 1, fmaxf(A.y, B.y) + trackRadius + 1);
        int minZ = (int)fmaxf(0.0f, fminf(A.z, B.z) - trackRadius - 1);
        int maxZ = (int)fminf(GRID_SIZE - 1, fmaxf(A.z, B.z) + trackRadius + 1);

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                for (int z = minZ; z <= maxZ; z++) {
                    vec3D P = { (float)x, (float)y, (float)z };
                    vec3D ap = connecting_vec(A, P);

                    float proj = dot_product(ap, ab) / (len_ab * len_ab);
                    proj = fmaxf(0.0f, fminf(1.0f, proj));

                    vec3D closestPoint = vec_add_scaled(A, proj, ab);
                    vec3D distVec = vec_minus(P, closestPoint);
                    float dist = vec_len(distVec);

                    float fieldVal = dist - trackRadius;
                    if (fieldVal < voxelGrid[x][y][z]) {
                        voxelGrid[x][y][z] = fieldVal;
                    }
                }
            }
        }
    }

    free(rawPoints);
}

// --- STEP 2: MARCHING CUBES ISO-SURFACE EXTRACTION ---
void generate_marching_cubes_mesh(float threshold) {
    numTriangles = 0;
    vec3D edgeVertices[12];

    for (int x = 0; x < GRID_SIZE - 1; x++) {
        for (int y = 0; y < GRID_SIZE - 1; y++) {
            for (int z = 0; z < GRID_SIZE - 1; z++) {

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

// --- OPENGL RENDERING AND INTERACTION ---
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
    glColor3f(0.2f, 0.7f, 0.9f);
    for (int i = 0; i < numTriangles; i++) {
        Triangle t = meshTriangles[i];

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
    gluPerspective(45.0, (float)w / h, 0.1, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        buttonDown = (state == GLUT_DOWN);
        lastX = x; lastY = y;
    }
}

void motion(int x, int y) {
    if (buttonDown) {
        camYaw += (x - lastX) * 0.2f;
        camPitch += (y - lastY) * 0.2f;
        lastX = x; lastY = y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    float speed = 5.0f;
    if (key == 'w') camZ -= speed;
    if (key == 's') camZ += speed;
    if (key == 'a') camX -= speed;
    if (key == 'd') camX += speed;
    if (key == ' ') camY += speed;
    if (key == 'c') camY -= speed;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Read directly from target path
    read_ncc_csv("./data/ncc_points.csv");

    generate_marching_cubes_mesh(0.0f);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("NCC Point Cloud Marching Cubes");

    if (glewInit() != GLEW_OK) {
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    free(meshTriangles);
    return 0;
}
