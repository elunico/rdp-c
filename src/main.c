#include "curve.h"
#include "curve_print.h"
#include "point.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  double epsilon = 0.075;
  if (argc == 2) {
    printf("%s\n", argv[1]);
    epsilon = atof(argv[1]);
  }

  curve c;

  const int pcount = 1000;

  c.points = malloc(sizeof(point) * (pcount + 1));
  c.length = 0;

  int index = 0;
  double start = -5;
  double end = 5;
  for (double d = -start; d < end; d += ((end - start) / pcount)) {
    c.points[index].x = d;
    c.points[index].y = exp(-d) * cos(2 * M_PI * d);
    c.length++;
    index++;
  }

  printf("start has length of %d\n", c.length);

  curve r = rdp(&c, epsilon);

  printf("epsilon=%f. result has %d points\n", epsilon, r.length);

  print(&c, NULL);

  rdp_result_free(&r);

  curve *q = curve_from_quadratic(5, 6, 5, -4, 4, 0.01);
  struct curve_print_properties props;
  props.start_x_0 = true;
  props.start_y_0 = true;
  props.symmetricX = true;
  props.symmetricY = true;
  print(q, &props);

  free(c.points);
  return 0;
}
