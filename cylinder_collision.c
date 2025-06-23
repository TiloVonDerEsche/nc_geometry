#include "helper_funcs.h"

//https://dfncloud.fh-kiel.de/s/prg56gjALs53QST

//fills the collision matricies M inside of the track structs
void track_collision(track** track_list, size_t track_list_len) {
  vec3D back_of_machine_vec = {0,-1,0};
  vec3D sky_vector = {0,0,1};

  size_t track_list_len = 10;

  track t1; track t2;
  vec3D tv;

  for (size_t i = 0; i < track_list_len; i++) {
    for (size_t j = 0; j < track_list_len; j++) {
      if (i != j) {
        t1 = (*track_list)[i];
        t2 = (*track_list)[j];

        //get the vector between t1 and t2
        //if all tracks are parallel
        //we can just compare the vector lens between all track starting points
        //-> btw t.A's


        tv = connecting_vec(t1.A,t2.A); //vector btw t1.A & t2.A

        //if the sum of both of the horizontal radii is greater than the
        //vec_len btw their starting points than squere's around
        // the A points would be colliding
        if ((t1.hradius + t2.hradius) >= vec_len(tv)) {


          //we orient the directions based on two common directions
          //1. back of the machine
          //2. vector pointing to the sky
          //, in the compass example
          //1. vector north
          //2. vector earth's core

          //when the tracks are parralel we can assume that all the vectors tv
          //are in the same plane, which is parallel to the back plane


          //naive approach, only works for one layer of tracks
          //t1 should be left to t2
          if (i < j) {
            t1.M.right = 1;
            t2.M.left = 1;
          }
          //t1 should be right of t2
          else { // (i > j)
            t1.M.left = 1;
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
