#ifndef CURVE_H
#define CURVE_H

#include "math.h"
#include "point.h"

typedef struct {
  int length;
  point *points;
} curve;

/**
 * Returns the index into the points of _inCurve_ where the furthest point is
 * and sets the value pointed to by _distance_ to that furthest distance.
 * Warning: _distance_ cannot be NULL
 */
int furthestPoint(curve const *inCurve, int start, int end, double *distance);

/**
 * Caller must free return value using rdp_result_free
 */
curve *rdp(curve const *start, double epsilon);

/**
 * Caller must free return value using curve_linear_free
 * Implementation note: delta is treated as a maximum.
 * points may be closer together than delta but will not
 * be further apart than delta
 */
curve *curve_from_line(double x1, double y1, double x2, double y2,
                       double delta);

/**
 * Caller must free return value using curve_quadratic_free
 * Implementation note: delta is treated as a maximum.
 * points may be closer together than delta but will not
 * be further apart than delta
 */
curve *curve_from_quadratic(double a, double b, double c, double xstart,
                            double xend, double delta);

/**
 * Caller must free return value using curve_construct_free
 * Implementation note: delta is treated as a maximum.
 * points may be closer together than delta but will not
 * be further apart than delta
 */
curve *curve_construct(double startX, double endX, double delta,
                       double (*f)(double));

void rdp_result_free(curve *c);
void curve_construct_free(curve *c);
void curve_quadratic_free(curve *c);
void curve_linear_free(curve *c);

#endif
