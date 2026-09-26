#define SUCCESS 0
#define FAILURE -1
#define MAX_COLOR_MAPS 10

#define TRUE 1
#define FALSE 0

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

typedef struct {
    unsigned int g_code;
    Color color;
} ColorMapEntry;

typedef struct {
  char tracks_to_plot[256];

  float horizontal_radius;
  float vertical_radius;

  int debug;

  Color default_color;
} Config;

// Structure to hold track data
typedef struct {
    unsigned int id;
    float ax, ay, az; // Start point t_start_x, t_start_y, t_start_z
    float bx, by, bz; // End point  t_end_x, ...
    float laser_power;
    float machine_speed;
    unsigned int g_code;

    Color color;
    float hradius, vradius; // Radii
} Track;
