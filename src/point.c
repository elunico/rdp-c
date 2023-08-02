#include "point.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

point fromangle(double rads) {
  point p;
  p.x = cos(rads);
  p.y = sin(rads);
  return p;
}

point mult(point const *p, double rhs) {
  point r;
  r.x = p->x * rhs;
  r.y = p->y * rhs;
  return r;
}

point fromangleandmag(double rads, double mag) {
  point p = fromangle(rads);
  return mult(&p, mag);
}

double dist(point const *a, point const *b) {
  double dx = a->x - b->x;
  double dy = a->y - b->y;

  return sqrt(dx * dx + dy * dy);
}

double dot(point const *a, point const *b) {
  double result = a->x * b->x + a->y * b->y;
  return result;
}

double mag(point const *p) { return sqrt((p->x * p->x) + (p->y * p->y)); }

void normalize(point *p) {
  double m = mag(p);
  assert(m != INFINITY);
  assert(m != 0);
  p->x = p->x / m;
  p->y = p->y / m;
}

void scalarProjection(point const *p, point const *a, point const *b,
                      point *destination) {
  point ap, ab;

  ap.x = p->x - a->x;
  ap.y = p->y - a->y;

  ab.x = b->x - a->x;
  ab.y = b->y - a->y;

  normalize(&ab);

  double sp = dot(&ap, &ab);
  ab.x *= sp;
  ab.y *= sp;

  ab.x += a->x;
  ab.y += a->y;

  *destination = ab;
}

double p2ldist(point const *p, point const *l1, point const *l2) {
  point norm;
  scalarProjection(p, l1, l2, &norm);
  return dist(p, &norm);
}
