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
  double delta = ((end - start) / pcount);
  printf("delta: %f\n", delta);
  for (double d = start; d < end; d += delta) {
    c.points[index].x = d;
    // c.points[index].y = exp(-d) * cos(2 * M_PI * d);
    c.points[index].y = tan(d / 3.25);
    c.length++;
    index++;
  }

  printf("start has length of %d\n", c.length);

  curve r = rdp(&c, epsilon);

  printf("epsilon=%f. result has %d points\n", epsilon, r.length);

  print(&c, NULL);

  rdp_result_free(&r);

  free(c.points);
  return 0;
}
