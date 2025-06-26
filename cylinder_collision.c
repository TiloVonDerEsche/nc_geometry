#include "helper_funcs.h"

//https://dfncloud.fh-kiel.de/s/prg56gjALs53QST

//fills the collision matricies M inside of the track structs
void track_collision(track** track_list, size_t track_list_len) {
  vec3D back_of_machine_vec = {0,-1,0};
  vec3D sky_vector = {0,0,1};

  size_t track_list_len = 10;

  track t1; track t2;
  float distance_t1_t2;

  for (size_t i = 0; i < track_list_len; i++) {
        //pairwise track extraction
        t1 = (*track_list)[i];
        t2 = (*track_list)[i+1];

        //assuming that the tracks are parallel -> point line distance
        distance_t1_t2 = distance_point_to_line(t1.A,t2.A,connecting_vec(t2.A,t2.B));

        if ((t1.hradius + t2.hradius) >= distance_t1_t2) {
          //Maybe we should get the Lotfußpunkt and compare t1.A with the Lotfußpunkt
          //on t2

          //TODO add margin around 0 to accommodate for floating point errors
          if (tv.x > 0) {
            t2.M.left = 1;
          }

          else {
            t2.M.right = 1;
          }



          //how do we determine the side the tracks are colliding on?
          //should we check the direction of the connecting_vec tv?
          //-> divide into 8 directions l, bl, b, br, r, tr, t, tl
          //sort of like a compass w, sw, s, se, e, ne, n, nw

          //getting the direction of tv
          // x, y > 0 -> top_right
          // x > 0 & y == 0 -> right
          // x > 0 & y < 0 -> bot_right
          // x == 0 & y < 0 -> bottom
          // x < 0 & y < 0 -> bot_left
          // x < 0 & y == 0 -> left
          // x < 0 & y > 0 -> top_left
          // x == 0  & y > 0 -> top

          printf("tv(%f %f %f)",tv.x,tv.y,tv.z);

        }
      }
    }
  }
}
