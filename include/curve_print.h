#ifndef CURVE_PRINT_H
#define CURVE_PRINT_H

#include "curve.h"
#include "point.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct curve_extrema {
  int valid;
  double xmin, xmax, ymin, ymax;
};

double map(double n, double start1, double stop1, double start2, double stop2);
void get_term_size(int *h, int *w);
struct curve_extrema get_curve_extrema(curve const *c);
void fix_bounds(double *min, double *max, bool sym, bool start0);
void print(curve const *c);

#endif
