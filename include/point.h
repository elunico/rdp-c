#ifndef POINT_H
#define POINT_H

typedef struct {
  double x, y;
} point;

point fromangle(double rads);

point mult(point const *p, double rhs);

point fromangleandmag(double rads, double mag);

double dist(point const *a, point const *b);

double dot(point const *a, point const *b);

double mag(point const *p);

point copy(point const *p);

/**
 * This function normalizes p as if it were a 2D vector
 * This function mutates p
 */
void normalize(point *p);

/**
 * This function calculates the scalarProjection of point p on the
 * line defined by endpoints a and b. The result is stored in
 * destination which may NOT be NULL
 */
void scalarProjection(point const *p, point const *a, point const *b,
                      point *destination);

/**
 * Returns the distance from point p to the line defined by the end
 * points l1 and l2.
 */
double p2ldist(point const *p, point const *l1, point const *l2);

#endif
