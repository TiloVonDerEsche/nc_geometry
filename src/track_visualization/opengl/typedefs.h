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
    float start_x, start_y, start_z; // Start point t_start_x, t_start_y, t_start_z
    float end_x, end_y, end_z; // End point  t_end_x, ...
    float b, c;
    float machine_speed;
    float laser_power;
    unsigned int laser; //laser on : off?
    unsigned int g_code;

    Color color;
    float hradius, vradius; // Radii
} Track;
