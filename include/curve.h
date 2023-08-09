#ifndef CURVE_H
#define CURVE_H

#include "math.h"
#include "point.h"

typedef struct {
  int length;
  point *points;
} curve;

int furthestPoint(curve const *inCurve, int start, int end, double *distance);

void rdp_support(curve const *original, double epsilon, int sidx, int eidx,
                 int *included, int *totalPoints);

curve rdp(curve const *start, double epsilon);

void rdp_result_free(curve *c);

curve *curve_from_line(double x1, double y1, double x2, double y2,
                       double delta);

curve *curve_from_quadratic(double a, double b, double c, double xstart,
                            double xend, double delta);

curve *curve_construct(double startX, double endX, double delta,
                       double (*f)(double));

void curve_construct_free(curve *c);
void curve_quadratic_free(curve *c);
void curve_linear_free(curve *c);

#endif
