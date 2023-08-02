#include "curve.h"
#include "point.h"
#include <assert.h>
#include <math.h>
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

  c.points = malloc(sizeof(point) * 501);
  c.length = 0;

  int index = 0;
  for (double d = 0; d < 5; d += 0.01) {
    c.points[index].x = d;
    c.points[index].y = exp(-d * cos(2 * M_PI * d));
    c.length++;
    index++;
  }

  printf("start has length of %d\n", c.length);

  curve r = rdp(&c, epsilon);

  printf("epsilon=%f. result has %d points\n", epsilon, r.length);

  rdp_result_free(&r);

  free(c.points);
  return 0;
}
