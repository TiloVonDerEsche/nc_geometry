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
#include "color_mapper.tab.h"

void apply_color_config(FILE* fp, Track* t_ptr);

#define SPEED 0.3f

// Camera variables
float camX = 0.0f, camY = 0.0f, camZ = 50.0f;
float camRoll = 0.0f, camPitch = 0.0f, camYaw = 0.0f;
int lastX = 0, lastY = 0;
char camString[64] = "Cam: {0.0, 0.0, 0.0}";

int buttonDown = 0;
unsigned int rate_of_change = 1;
char rocString[32] = "Rate of Change: 1";
int keys[256] = {0};

// Track data
Track* tracks = NULL;
int numTracks = 0;
float min_z, max_z;

Config config = {0};

// Ambient Light Control Variable
float ambientIntensity = 0.2f;
char ambientString[32] = "Ambient Light Intensity: 0.2";

int frameCount = 0;
float fps = 0.0f;
int previousTime = 0;
char fpsString[16] = "FPS: 0";

unsigned int current_TID = 0;
char TID_String[16] = "T_ID: 0";

// VBO Handles for Cylinder Template
GLuint cylinderVBO = 0;
GLuint cylinderNormalVBO = 0;
int cylinderVertexCount = 0;

/**************************File / Str Functions**************************************/

int read_config(const char* filename, Config* config) {
    printf("Opening %s...\n", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open config file %s\n", filename);
        return FAILURE;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);

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
        } else if (strcmp(key, "debug_prints") == 0) {
            config->debug = atoi(value);
        } else if (strcmp(key, "default_color") == 0) {
            if(sscanf(value, "{%hhu,%hhu,%hhu}",
            &config->default_color.r, &config->default_color.g, &config->default_color.b) != 3) {
                fprintf(stderr,"Failed to read default_color!\n");
            } else{
                printf("Read default_color={%hhu,%hhu,%hhu}\n",
                    config->default_color.r, config->default_color.g, config->default_color.b);
            }
        }
    }

    fclose(file);
    return SUCCESS;
}

void read_track_list(const char* filename) {
    printf("Opening %s...\n", filename);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open %s\n", filename);
        exit(1);
    }

    const char* fname="color_config.ctxt";
    printf("Opening %s...\n", fname);
    FILE* colorf = fopen(fname, "r");
    if (colorf == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", fname);
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header

    int count = 0;
    while (fgets(line, sizeof(line), file)) count++;
    numTracks = count;
    tracks = (Track*)calloc(numTracks, sizeof(Track));
    rewind(file);
    fgets(line, sizeof(line), file); // Skip header again

    int i = 0;
    while (fgets(line, sizeof(line), file) && i < numTracks) {
        sscanf(line, "%u,%f,%f,%f, %f,%f,%f, %f,%f, %f, %f,%u, %u",
               &tracks[i].id,
               &tracks[i].start_x, &tracks[i].start_y, &tracks[i].start_z,
               &tracks[i].end_x, &tracks[i].end_y, &tracks[i].end_z,
               &tracks[i].b, &tracks[i].c,
               &tracks[i].machine_speed,
               &tracks[i].laser_power, &tracks[i].laser_status,
               &tracks[i].g_code);
        tracks[i].hradius = config.horizontal_radius;
        tracks[i].vradius = config.vertical_radius;
        tracks[i].color = config.default_color;

        apply_color_config(colorf, &tracks[i]);
        rewind(colorf);

        i++;
    }
    fclose(file);
}

/****************************************************************/

void initCylinderVBO() {
    const int segments = 12; // Balanced detail vs performance
    cylinderVertexCount = segments * 6; // 2 triangles per segment * 3 vertices

    float* vertices = (float*)malloc(cylinderVertexCount * 3 * sizeof(float));
    float* normals = (float*)malloc(cylinderVertexCount * 3 * sizeof(float));

    int v_idx = 0;
    for (int i = 0; i < segments; i++) {
        float theta1 = (float)i * 2.0f * M_PI / segments;
        float theta2 = (float)(i + 1) * 2.0f * M_PI / segments;

        float x1 = cosf(theta1), y1 = sinf(theta1);
        float x2 = cosf(theta2), y2 = sinf(theta2);

        // Triangle 1: Bottom-Left -> Top-Left -> Bottom-Right
        vertices[v_idx*3 + 0] = x1; vertices[v_idx*3 + 1] = y1; vertices[v_idx*3 + 2] = 0.0f;
        normals[v_idx*3 + 0] = x1;  normals[v_idx*3 + 1] = y1;  normals[v_idx*3 + 2] = 0.0f;
        v_idx++;

        vertices[v_idx*3 + 0] = x1; vertices[v_idx*3 + 1] = y1; vertices[v_idx*3 + 2] = 1.0f;
        normals[v_idx*3 + 0] = x1;  normals[v_idx*3 + 1] = y1;  normals[v_idx*3 + 2] = 0.0f;
        v_idx++;

        vertices[v_idx*3 + 0] = x2; vertices[v_idx*3 + 1] = y2; vertices[v_idx*3 + 2] = 0.0f;
        normals[v_idx*3 + 0] = x2;  normals[v_idx*3 + 1] = y2;  normals[v_idx*3 + 2] = 0.0f;
        v_idx++;

        // Triangle 2: Bottom-Right -> Top-Left -> Top-Right
        vertices[v_idx*3 + 0] = x2; vertices[v_idx*3 + 1] = y2; vertices[v_idx*3 + 2] = 0.0f;
        normals[v_idx*3 + 0] = x2;  normals[v_idx*3 + 1] = y2;  normals[v_idx*3 + 2] = 0.0f;
        v_idx++;

        vertices[v_idx*3 + 0] = x1; vertices[v_idx*3 + 1] = y1; vertices[v_idx*3 + 2] = 1.0f;
        normals[v_idx*3 + 0] = x1;  normals[v_idx*3 + 1] = y1;  normals[v_idx*3 + 2] = 0.0f;
        v_idx++;

        vertices[v_idx*3 + 0] = x2; vertices[v_idx*3 + 1] = y2; vertices[v_idx*3 + 2] = 1.0f;
        normals[v_idx*3 + 0] = x2;  normals[v_idx*3 + 1] = y2;  normals[v_idx*3 + 2] = 0.0f;
        v_idx++;
    }

    glGenBuffers(1, &cylinderVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &cylinderNormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderNormalVBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);

    free(vertices);
    free(normals);
}

void drawCylinder(float ax, float ay, float az, float bx, float by, float bz, float hradius, float vradius) {
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

    // Bind VBOs and draw using client arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderNormalVBO);
    glNormalPointer(GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, cylinderVertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopMatrix();
}

void renderText() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.0f, 1.0f, 0.0f);

    glRasterPos2i(10, height - 20);
    for (char* c = fpsString; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glRasterPos2i(10, height - 40);
    for (char* c = camString; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glRasterPos2i(10, height - 60);
    for (char* c = rocString; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glRasterPos2i(10, height - 80);
    for (char* c = TID_String; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glRasterPos2i(10, height - 100);
    for (char* c = ambientString; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void display() {
    frameCount++;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeInterval = currentTime - previousTime;

    if (timeInterval > 1000) {
        fps = frameCount / (timeInterval / 1000.0f);
        previousTime = currentTime;
        frameCount = 0;
        snprintf(fpsString, sizeof(fpsString), "FPS: %.1f", fps);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GLfloat ambientLight[] = {ambientIntensity, ambientIntensity, ambientIntensity, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Camera
    glRotatef(camPitch, 1, 0, 0);
    glRotatef(camYaw, 0, 1, 0);
    glTranslatef(-camX, -camY, -camZ);

    // Draw tracks
    for (int i = 0; i < numTracks; i++) {
        glColor3ub(tracks[i].color.r, tracks[i].color.g, tracks[i].color.b);
        if(tracks[i].id < current_TID) {
            if(tracks[i].color.r!=0 || tracks[i].color.g!=0 || tracks[i].color.b!=0) {
                drawCylinder(tracks[i].start_x, tracks[i].start_y, tracks[i].start_z,
                             tracks[i].end_x, tracks[i].end_y, tracks[i].end_z,
                             tracks[i].hradius, tracks[i].vradius);
            }
        }
    }
    renderText();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouseEntry(int state) {
    if (state == GLUT_ENTERED) {
        rate_of_change = 1;
        for (int i = 0; i < 256; i++) keys[i] = 0;
    }
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
}

void keyUp(unsigned char key, int x, int y) {
    keys[tolower(key)] = 0;
}

void specialKeyDown(int key, int x, int y) {
  if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) {
      rate_of_change*=100;
  }
  if (key == GLUT_KEY_SHIFT_L || key == GLUT_KEY_SHIFT_R) {
      rate_of_change*=10;
  }

  unsigned int current_TID_changed=0;

  if (key == GLUT_KEY_LEFT) {
      current_TID -= rate_of_change;
      if(numTracks<=current_TID) {
        current_TID=numTracks;
      }
      current_TID_changed=1;
  } else if (key == GLUT_KEY_RIGHT) {
      current_TID += rate_of_change;
      if(numTracks<=current_TID) {
        current_TID=0;
      }
      current_TID_changed=1;
  }

  if (current_TID_changed) {
    snprintf(TID_String, sizeof(TID_String), "T_ID: %u", current_TID);
  }
}

void specialKeyUp(int key, int x, int y) {
    if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) {
        rate_of_change/=100;
    }
    if (key == GLUT_KEY_SHIFT_L || key == GLUT_KEY_SHIFT_R) {
        rate_of_change/=10;
    }

    if (rate_of_change<1) {rate_of_change=1;}
}

void handle_movement(int garbage) {
    float speed = SPEED * rate_of_change;

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

    unsigned int ambient_changed=0;

    if (keys['i']) {
        ambientIntensity += 0.01f;
        if (ambientIntensity > 1.0f) ambientIntensity = 1.0f;
        ambient_changed=1;
    } else if (keys['k']) {
        ambientIntensity -= 0.01f;
        if (ambientIntensity < 0.0f) ambientIntensity = 0.0f;
        ambient_changed=1;
    }

    if (ambient_changed) {
      snprintf(ambientString, sizeof(ambientString), "Ambient Light Intensity: %.2f", ambientIntensity);
    }

    snprintf(camString, sizeof(camString), "Cam: {%.2f, %.2f, %.2f}", camX,camY,camZ);
    snprintf(rocString, sizeof(rocString), "Rate of Change: %u", rate_of_change);

    glutPostRedisplay();
    glutTimerFunc(16, handle_movement, 0);
}

int main(int argc, char** argv) {
    read_config("plot_config.ctxt", &config);
    read_track_list(config.tracks_to_plot);

    min_z = tracks[0].start_z;
    max_z = tracks[0].start_z;
    for (int i = 0; i < numTracks; i++) {
        if (tracks[i].start_z < min_z) min_z = tracks[i].start_z;
        if (tracks[i].start_z > max_z) max_z = tracks[i].start_z;
        if (tracks[i].end_z < min_z) min_z = tracks[i].end_z;
        if (tracks[i].end_z > max_z) max_z = tracks[i].end_z;
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
    glEnable(GL_NORMALIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat diffuseLight[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat specularMaterial[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[]        = {50.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // Initialize VBO templates on GPU after GLEW setup
    initCylinderVBO();

    // GLUT Callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutEntryFunc(mouseEntry);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutTimerFunc(0, handle_movement, 0);

    glutMainLoop();

    free(tracks);
    return 0;
}
