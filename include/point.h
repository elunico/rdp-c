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

void normalize(point *p);

void scalarProjection(point const *p, point const *a, point const *b,
                      point *destination);

double p2ldist(point const *p, point const *l1, point const *l2);

#endif
