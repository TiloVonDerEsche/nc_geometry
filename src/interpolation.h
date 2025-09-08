

void linear_intpol(size_t tl_len, track** tl, Config* config) {

  FILE* intpol_csv = fopen(config->intpol_csv, "w"); // or "a" to append
  if (intpol_csv == NULL){
      fprintf(stderr, "An error occured, while trying to open %s (in write mode)!\n", config->intpol_csv);
      return;
  }
  printf("Opened %s in write mode...\n",config->intpol_csv);


  track t; // = (*tl)[0];
  vec3D start; // = t.SP;
  vec3D end; // = t.EP;

  //get vector start->end
  vec3D SE; // = vec_minus(end,start);
  float SE_len; // = vec_len(SE);


  //get step vector
  vec3D track_dir; // = norm_vec(SE);

  //vector with length of step_dis in direction of track (start->end)
  vec3D step_vec; // = vec_scale(track_dir, config->step_dis);

  //linear interpolation point
  vec3D lip; //{0,0,0};
  vec3D P; //vec_add(start,lip);



  for (size_t ti = 0; ti < tl_len; ti++) {
    //prepare intpol for new track
    t = (*tl)[ti];
    start = t.SP;
    end = t.EP;

    SE = vec_minus(end,start);
    SE_len = vec_len(SE);

    track_dir = norm_vec(SE);

    step_vec = vec_scale(track_dir, config->step_dis);

    lip = (vec3D) {0,0,0};
    printf("Interpolating track %lu...\n", (ti+1));

    while (vec_len(lip) < SE_len) {
      P = vec_add(start,lip);


      printf("(%f, %f, %f)\n",P.x, P.y, P.z);
      fprintf(intpol_csv, "%f, %f, %f\n",P.x, P.y, P.z);

      //take step on track
      lip = vec_add(lip, step_vec);
    }
  }
}
