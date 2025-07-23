

//https://dfncloud.fh-kiel.de/s/prg56gjALs53QST

//assigns the coll_vec inside of the track structs
void track_collision(size_t track_list_len, track** track_list) {
  if (track_list_len < 2) {puts("LESS THAN TWO TRACKS IN TRACK LIST!");return;}
  track* t1; track* t2;

  double distance_t1_t2;
  vec3D foot_point;
  vec3D v;

  vec3D FQ;
  vec3D FQ_normed;

  for (size_t i = 0; i < track_list_len; i++) {
        //pairwise track extraction
        t1 = &(*track_list)[i];
        t2 = &(*track_list)[i+1];

        //assuming that the tracks are parallel -> point line distance
        //distance_point_to_line(t1->A,t2->A,connecting_vec(t2->A,t2->B));
        v = vec_minus(t1->B,t1->A);
        FQ = lotfuss(t1->A,v,t2->A, &foot_point, &distance_t1_t2);

        FQ_normed = norm_vec(FQ);

        if ((t1->hradius + t2->hradius) >= distance_t1_t2) {

          t2->coll_vec = FQ_normed;

          // if (tv.y > 0) {
          //   //puts("LEFT");
          //   t2->coll_vec.x = -1;
          // }
          //
          // else if (tv.y < 0) {
          //   //puts("RIGHT");
          //   t2->coll_vec.x = 1;
          // }

          //getting the direction of tv
          // x, y > 0 -> top_right
          // x > 0 & y == 0 -> right
          // x > 0 & y < 0 -> bot_right
          // x == 0 & y < 0 -> bottom
          // x < 0 & y < 0 -> bot_left
          // x < 0 & y == 0 -> left
          // x < 0 & y > 0 -> top_left
          // x == 0  & y > 0 -> top

        }
  }
}
