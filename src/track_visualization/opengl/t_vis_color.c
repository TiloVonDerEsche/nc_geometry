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
#include "shaders_embedded.h"
#include "cm/cm.h" //color_mapper

//void apply_color_config(FILE* fp, Track* t_ptr);

int compile_color_config(FILE* fp);
void map_color(Track* t);

#define SPEED 0.3f

// Camera variables (will be initialized around track 0 in main)
float camX = 0.0f, camY = 0.0f, camZ = 50.0f;
float camRoll = 0.0f, camPitch = 15.0f, camYaw = 0.0f;
int lastX = 0, lastY = 0;


char camStrPos[32] =     "Pos: (0.0, 0.0, 0.0)";
char camStrTarget[32] =  "Target: (0.0, 0.0, 0.0)";
char camStrYaw[32] =     "Yaw: 0.0°";
char camStrPitch[32] =   "Pitch: 0.0°";
char camStrTYaw[32] =    "Target Yaw: 0.0°";
char camStrTPitch[32] =  "Target Pitch: 0.0°";
char camStrDir[32] =     "Direction: (0.0, 0.0, 0.0)";
char camStrDis[32] =     "Distance: 0.0";
char camStrForward[32] = "Forward: (0.0, 0.0, 0.0)";

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

unsigned int current_TID = 0; // Starts at 0 as requested
char TID_String[16] = "T_ID: 0";

// OpenGL / Instancing Handles
GLuint cylinderVBO = 0;
GLuint cylinderNormalVBO = 0;
GLuint instanceVBO = 0;
GLuint shaderProgram = 0;
int cylinderVertexCount = 0;

// Instance data structure for GPU
typedef struct {
    float start[3];
    float end[3];
    float radii[2]; // hradius, vradius
    float color[3];
} TrackInstanceData;

/************************** GLSL Shaders **************************************/
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    const char* typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: %s shader compilation failed:\n%s\n", typeStr, infoLog);
    } else {
        printf("SUCCESS: %s shader compiled successfully.\n", typeStr);
    }
    return shader;
}

void initShaders() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vs);
    printf("DEBUG: Vertex shader attached to program ID %u.\n", shaderProgram);

    glAttachShader(shaderProgram, fs);
    printf("DEBUG: Fragment shader attached to program ID %u.\n", shaderProgram);

    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: Shader program linking failed:\n%s\n", infoLog);
    } else {
        printf("SUCCESS: Shader program linked successfully (Program ID: %u).\n", shaderProgram);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

/************************** File / Str Functions **********************************/

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
        if (!key || !value) continue;

        if (strcmp(key, "tracks_to_plot") == 0) {
            strncpy(config->tracks_to_plot, value, sizeof(config->tracks_to_plot) - 1);
            config->tracks_to_plot[sizeof(config->tracks_to_plot) - 1] = '\0';
        } else if (strcmp(key, "horizontal_radius") == 0) {
            config->horizontal_radius = atof(value);
        } else if (strcmp(key, "vertical_radius") == 0) {
            config->vertical_radius = atof(value);
        } else if (strcmp(key, "debug_prints") == 0) {
            config->debug = atoi(value);
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

    const char* fname = "color.cconf";
    FILE* colorf = fopen(fname, "r");
    if(compile_color_config(colorf) == SUCCESS) {
      printf("Compiled %s successfully!\n",fname);
    } else {
      fprintf(stderr,"Failed to compile %s!\n",fname);
      //NOTE should we exit here?
      exit(1);
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header

    int count = 0;
    //TODO Replace with fseek end, see NOTE
    while (fgets(line, sizeof(line), file)) count++;
    numTracks = count;
    tracks = (Track*)calloc(numTracks, sizeof(Track));
    rewind(file); //NOTE seek END and then rewind should be better
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
        tracks[i].color = (Color){0,0,0};
        map_color(&tracks[i]); //NOTE Uses compiled color_mapper AST
        i++;
    }
    fclose(file);
    if (colorf) fclose(colorf);
}

/****************************************************************/

void initInstancedBuffers() {
    const int segments = 12;
    cylinderVertexCount = segments * 6;

    float* vertices = (float*)malloc(cylinderVertexCount * 3 * sizeof(float));
    float* normals = (float*)malloc(cylinderVertexCount * 3 * sizeof(float));

    int v_idx = 0;
    for (int i = 0; i < segments; i++) {
        float theta1 = (float)i * 2.0f * M_PI / segments;
        float theta2 = (float)(i + 1) * 2.0f * M_PI / segments;

        float x1 = cosf(theta1), y1 = sinf(theta1);
        float x2 = cosf(theta2), y2 = sinf(theta2);

        // Triangle 1
        vertices[v_idx*3 + 0] = x1; vertices[v_idx*3 + 1] = y1; vertices[v_idx*3 + 2] = 0.0f;
        normals[v_idx*3 + 0]  = x1; normals[v_idx*3 + 1]  = y1; normals[v_idx*3 + 2]  = 0.0f;
        v_idx++;
        vertices[v_idx*3 + 0] = x1; vertices[v_idx*3 + 1] = y1; vertices[v_idx*3 + 2] = 1.0f;
        normals[v_idx*3 + 0]  = x1; normals[v_idx*3 + 1]  = y1; normals[v_idx*3 + 2]  = 0.0f;
        v_idx++;
        vertices[v_idx*3 + 0] = x2; vertices[v_idx*3 + 1] = y2; vertices[v_idx*3 + 2] = 0.0f;
        normals[v_idx*3 + 0]  = x2; normals[v_idx*3 + 1]  = y2; normals[v_idx*3 + 2]  = 0.0f;
        v_idx++;

        // Triangle 2
        vertices[v_idx*3 + 0] = x2; vertices[v_idx*3 + 1] = y2; vertices[v_idx*3 + 2] = 0.0f;
        normals[v_idx*3 + 0]  = x2; normals[v_idx*3 + 1]  = y2; normals[v_idx*3 + 2]  = 0.0f;
        v_idx++;
        vertices[v_idx*3 + 0] = x1; vertices[v_idx*3 + 1] = y1; vertices[v_idx*3 + 2] = 1.0f;
        normals[v_idx*3 + 0]  = x1; normals[v_idx*3 + 1]  = y1; normals[v_idx*3 + 2]  = 0.0f;
        v_idx++;
        vertices[v_idx*3 + 0] = x2; vertices[v_idx*3 + 1] = y2; vertices[v_idx*3 + 2] = 1.0f;
        normals[v_idx*3 + 0]  = x2; normals[v_idx*3 + 1]  = y2; normals[v_idx*3 + 2]  = 0.0f;
        v_idx++;
    }

    // Base mesh buffers
    glGenBuffers(1, &cylinderVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &cylinderNormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderNormalVBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);

    free(vertices);
    free(normals);

    // Build Instance Data Array
    TrackInstanceData* instanceData = (TrackInstanceData*)malloc(numTracks * sizeof(TrackInstanceData));
    int validCount = 0;
    for (int i = 0; i < numTracks; i++) {
        instanceData[validCount].start[0] = tracks[i].start_x;
        instanceData[validCount].start[1] = tracks[i].start_y;
        instanceData[validCount].start[2] = tracks[i].start_z;

        instanceData[validCount].end[0] = tracks[i].end_x;
        instanceData[validCount].end[1] = tracks[i].end_y;
        instanceData[validCount].end[2] = tracks[i].end_z;

        if (tracks[i].color.r == 0 && tracks[i].color.g == 0 && tracks[i].color.b == 0) {
            instanceData[validCount].color[0] = 0.0f;
            instanceData[validCount].color[1] = 0.0f;
            instanceData[validCount].color[2] = 0.0f;

            //vertex shader will calculate a zero-sized cylinder,
            //GPU rasterizer will discard it
            //-> do not render tracks with color {0,0,0}
            instanceData[validCount].radii[0] = 0.0f;
            instanceData[validCount].radii[1] = 0.0f;
        } else {
            instanceData[validCount].color[0] = tracks[i].color.r / 255.0f;
            instanceData[validCount].color[1] = tracks[i].color.g / 255.0f;
            instanceData[validCount].color[2] = tracks[i].color.b / 255.0f;

            instanceData[validCount].radii[0] = tracks[i].hradius;
            instanceData[validCount].radii[1] = tracks[i].vradius;
        }
        validCount++;
    }

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, numTracks * sizeof(TrackInstanceData), instanceData, GL_STATIC_DRAW);
    free(instanceData);
}

void cameraText(void)
{
    int idx = current_TID;

    // Vector from camera position to track 0's start point.
    float dx = tracks[idx].start_x - camX;
    float dy = tracks[idx].start_y - camY;
    float dz = tracks[idx].start_z - camZ;

    float horizontal = sqrtf(dx * dx + dy * dy);
    float distance = sqrtf(dx * dx + dy * dy + dz * dz);

    // Z-up convention:
    // yaw = angle in XY plane, measured from +Y toward +X
    // pitch = elevation above the XY plane
    float targetYaw = atan2f(dx, dy) * 180.0f / (float)M_PI;
    float targetPitch = atan2f(dz, horizontal)
                      * 180.0f / (float)M_PI;

    // Direction vector from camera toward track start.
    float dirX = 0.0f, dirY = 0.0f, dirZ = 0.0f;
    if (distance > 1e-6f) {
        dirX = dx / distance;
        dirY = dy / distance;
        dirZ = dz / distance;
    }

    float yawRad = camYaw * (float)M_PI / 180.0f;
    float pitchRad = camPitch * (float)M_PI / 180.0f;

    // Forward vector consistent with yaw/pitch convention
    float forwardX = sinf(yawRad) * cosf(pitchRad);
    float forwardY = cosf(yawRad) * cosf(pitchRad);
    float forwardZ = sinf(pitchRad);

    snprintf(camStrPos, sizeof(camStrPos), "Pos: (%.2f, %.2f, %.2f)",
      camX, camY, camZ);
    snprintf(camStrTarget, sizeof(camStrTarget), "Target: (%.2f, %.2f, %.2f)",
      tracks[idx].start_x,tracks[idx].start_y,tracks[idx].start_z);
    snprintf(camStrYaw, sizeof(camStrYaw), "Yaw: %.2f°",
      camYaw);
    snprintf(camStrPitch, sizeof(camStrPitch), "Pitch: %.2f°",
      camPitch);
    snprintf(camStrTYaw, sizeof(camStrTYaw), "Target Yaw: %.2f°",
      targetYaw);
    snprintf(camStrTPitch, sizeof(camStrTPitch), "Target Pitch: %.2f°",
      targetPitch);
    snprintf(camStrDir, sizeof(camStrDir), "Direction: (%.2f, %.2f, %.2f)",
      dirX, dirY, dirZ);
    snprintf(camStrDis, sizeof(camStrDis), "Distance: %.2f",
      distance);
    snprintf(camStrForward, sizeof(camStrForward), "Forward: (%.2f, %.2f, %.2f)",
      forwardX, forwardY, forwardZ);
}

void renderText() {
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
    for (char* c = fpsString; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 40);
    for (char* c = rocString; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 60);
    for (char* c = TID_String; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 80);
    for (char* c = ambientString; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 120);
    for (char* c = camStrPos; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 140);
    for (char* c = camStrTarget; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 160);
    for (char* c = camStrYaw; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 180);
    for (char* c = camStrPitch; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 200);
    for (char* c = camStrTYaw; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 220);
    for (char* c = camStrTPitch; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 240);
    for (char* c = camStrDir; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 260);
    for (char* c = camStrDis; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glRasterPos2i(10, height - 280);
    for (char* c = camStrForward; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
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

    float aspect = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);

    // Projection Matrix (gluPerspective equivalent)
    float fov = 45.0f * M_PI / 180.0f;
    float f = 1.0f / tanf(fov / 2.0f);
    float zNear = 0.1f, zFar = 1000.0f;
    GLfloat projMat[16] = {
        f / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (zFar + zNear) / (zNear - zFar), -1.0f,
        0.0f, 0.0f, (2.0f * zFar * zNear) / (zNear - zFar), 0.0f
    };

    // View Matrix (Camera rotation + translation)
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(camYaw, 0, 0, 1);
    glRotatef(camPitch, 1, 0, 0);
    glTranslatef(-camX, -camY, -camZ);
    GLfloat viewMat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, viewMat);*/
    // View Matrix: Z-up coordinate system.
    // Yaw 0 looks along +Y.
    // Positive yaw turns toward +X.
    // Positive pitch looks upward toward +Z.

    float yawRad = camYaw * (float)M_PI / 180.0f;
    float pitchRad = camPitch * (float)M_PI / 180.0f;

    float forwardX = sinf(yawRad) * cosf(pitchRad);
    float forwardY = cosf(yawRad) * cosf(pitchRad);
    float forwardZ = sinf(pitchRad);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        camX, camY, camZ,
        camX + forwardX,
        camY + forwardY,
        camZ + forwardZ,
        0.0f, 0.0f, 1.0f  // Z is up
    );

    GLfloat viewMat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, viewMat);


    // Use Custom Shader Program
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projMat);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, viewMat);
    glUniform1f(glGetUniformLocation(shaderProgram, "ambientIntensity"), ambientIntensity);

    // Bind Cylinder Mesh & Instance Attributes
    glEnableVertexAttribArray(0); // aPos
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1); // aNormal
    glBindBuffer(GL_ARRAY_BUFFER, cylinderNormalVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Instance attributes from instanceVBO
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    size_t stride = sizeof(TrackInstanceData);

    glEnableVertexAttribArray(2); // aStart
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3); // aEnd
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4); // aRadii
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5); // aColor
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
    glVertexAttribDivisor(5, 1);

    // Draw all instances up to current_TID in a single call!
    unsigned int drawCount = (current_TID < numTracks) ? current_TID : numTracks;
    if (drawCount > 0) {
        glDrawArraysInstanced(GL_TRIANGLES, 0, cylinderVertexCount, drawCount);
    }

    // Reset divisors
    glVertexAttribDivisor(2, 0);
    glVertexAttribDivisor(3, 0);
    glVertexAttribDivisor(4, 0);
    glVertexAttribDivisor(5, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glUseProgram(0);

    cameraText();
    renderText();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void mouseEntry(int state) {
    if (state == GLUT_ENTERED) {
        rate_of_change = 1;
        for (int i = 0; i < 256; i++) keys[i] = 0;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        buttonDown = (state == GLUT_DOWN);
        lastX = x;
        lastY = y;
    }
}

void motion(int x, int y) {
    if (buttonDown) {
        camYaw += (x - lastX) * 0.1f;
        camPitch -= (y - lastY) * 0.1f;
        if (camPitch > 89.0f) camPitch = 89.0f;
        if (camPitch < -89.0f) camPitch = -89.0f;
        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

void keyDown(unsigned char key, int x, int y) { keys[tolower(key)] = 1; }
void keyUp(unsigned char key, int x, int y) { keys[tolower(key)] = 0; }

void specialKeyDown(int key, int x, int y) {
    // mode keys
    if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) {
        rate_of_change *= 100;
    }
    if (key == GLUT_KEY_SHIFT_L || key == GLUT_KEY_SHIFT_R) {
        rate_of_change *= 10;
    }

    // arrow keys
    unsigned int current_TID_changed = 0;

    if (key == GLUT_KEY_LEFT) {
        current_TID -= rate_of_change;
        if (numTracks <= current_TID) { // underflow feature
            current_TID = numTracks;
        }
        current_TID_changed = 1;
    } else if (key == GLUT_KEY_RIGHT) {
        current_TID += rate_of_change;
        if (numTracks <= current_TID) { // overflow feature
            current_TID = 0;
        }
        current_TID_changed = 1;
    }

    if (current_TID_changed) {
        snprintf(TID_String, sizeof(TID_String), "T_ID: %u", current_TID);
    }
}

void specialKeyUp(int key, int x, int y) {
    // mode keys
    if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) {
        rate_of_change /= 100;
    }
    if (key == GLUT_KEY_SHIFT_L || key == GLUT_KEY_SHIFT_R) {
        rate_of_change /= 10;
    }

    if (rate_of_change < 1) { rate_of_change = 1; } // lower clamp to 1
}

void handle_movement(int garbage) {
    float speed = SPEED * rate_of_change;
    float yawRad = camYaw * M_PI / 180.0f;
    float forwardX = sinf(yawRad), forwardY = cosf(yawRad);
    float rightX = cosf(yawRad), rightY = -sinf(yawRad);

    if (keys['w']) { camX += forwardX * speed; camY += forwardY * speed; }
    if (keys['s']) { camX -= forwardX * speed; camY -= forwardY * speed; }
    if (keys['a']) { camX -= rightX * speed; camY -= rightY * speed; }
    if (keys['d']) { camX += rightX * speed; camY += rightY * speed; }
    if (keys[' ']) { camZ += speed; }
    if (keys['c']) { camZ -= speed; }

    unsigned int ambient_changed = 0;
    if (keys['i']) { ambientIntensity += 0.01f; if (ambientIntensity > 1.0f) ambientIntensity = 1.0f; ambient_changed = 1; }
    else if (keys['k']) { ambientIntensity -= 0.01f; if (ambientIntensity < 0.0f) ambientIntensity = 0.0f; ambient_changed = 1; }

    if (ambient_changed) {
        snprintf(ambientString, sizeof(ambientString), "Ambient Light Intensity: %.2f", ambientIntensity);
    }

    snprintf(rocString, sizeof(rocString), "Rate of Change: %u", rate_of_change);

    glutPostRedisplay();
    glutTimerFunc(16, handle_movement, 0);
}

int main(int argc, char** argv) {
    read_config("plot.cconf", &config);
    read_track_list(config.tracks_to_plot);

    // Initialize camera facing track 0's start point
    if (numTracks > 0) {
        camX = tracks[0].start_x;
        camY = tracks[0].start_y; // Position camera back along Y
        camZ = tracks[0].start_z; // Position slightly above Z
        camPitch = 0.0f;
        camYaw = 0.0f;
    }

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
    glutCreateWindow("3D Track Visualizer - Instanced");

    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed\n");
        return 1;
    }
    const GLubyte* version = glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", version);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initialize Shaders and GPU Instanced Buffers
    initShaders();
    initInstancedBuffers();

    //register callbacks
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
