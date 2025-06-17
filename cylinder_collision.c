#include "helper_funcs.h"

vec3D back_of_machine_vec = {0,-1,0};
vec3D sky_vector = {0,0,1};

size_t cyls_len = 10;

for (size_t i = 0; i < cyls_len; i++) {
  for (size_t j = 0; j < cyls_len; j++) {
    if (i != j) {
      t1 = cyls[i];
      t2 = cyls[j];

      //get the vector between t1 and t2
      //if all tracks are parallel
      //we can just compare the vector lens between all track starting points
      //-> btw t.A's


      vec3D tv = connecting_vec(t1.A,t2.A); //vector btw t1.A & t2.A

      //if the sum of both of the horizontal radii is greater than the
      //vec_len btw their starting points than squere's around
      // the A points would be colliding
      if ((t1.hradius + t2.hradius) >= vec_len(tv)) {
        //how do we determine the side the tracks are colliding on?
        //should we check the direction of the connecting_vec tv?
        //-> divide into 8 directions l, bl, b, br, r, tr, t, tl
        //sort of like a compass w, sw, s, se, e, ne, n, nw

        //we orient the directions based on two common directions
        //1. back of the machine
        //2. vector pointing to the sky
        //, in the compass example
        //1. vector north
        //2. vector earth's core

        //when the tracks are parralel we can assume that all the vectors tv
        //are in the same plane, which is parallel to the back plane

      }
    }
  }
}
