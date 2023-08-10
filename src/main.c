#include "curve.h"
#include "curve_print.h"
#include "point.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double tanFunc(double x) { return tan(x / 3.25); }

int main(int argc, char const *argv[]) {
  double epsilon = 0.075;
  if (argc == 2) {
    printf("%s\n", argv[1]);
    epsilon = atof(argv[1]);
  }

  curve *c = curve_construct(-5, 5, 0.01, tanFunc);

  printf("start has length of %d\n", c->length);

  curve *r = rdp(c, epsilon);

  printf("epsilon=%f. result has %d points\n", epsilon, r->length);

  curve_print(c, NULL);

  rdp_result_free(r);

  curve_construct_free(c);
  // free(c.points);
  return 0;
}
