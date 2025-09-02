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
        v = vec_minus(t1->EP,t1->SP);
        FQ = lotfuss(t1->SP,v,t2->SP, &foot_point, &distance_t1_t2);

        FQ_normed = norm_vec(FQ);

        if ((t1->hradius + t2->hradius) >= distance_t1_t2) {

          t2->coll_vec = FQ_normed;
        }
  }
}
