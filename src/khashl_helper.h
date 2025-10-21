void add_laser_var(strfloat_t* h) {
  int absent;
  khint_t k = strfloat_put(h, "laser", &absent);

  if (absent) {
    kh_key(h, k) = strdup("laser");}

  kh_val(h, k) = 0;
}
