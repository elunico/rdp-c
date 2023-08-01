#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  double x, y;
} point;

typedef struct {
  int length;
  point *points;
} curve;

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

int furthestPoint(curve const *inCurve, int start, int end, double *distance) {

  int furthestIndex = -1;
  double recordDist = 0;

  point *s = &inCurve->points[start];
  point *e = &inCurve->points[end];
  for (int i = start + 1; i < end; i++) {
    double t = p2ldist(&inCurve->points[i], s, e);
    if (t > recordDist) {
      furthestIndex = i;
      recordDist = t;
      *distance = recordDist;
    }
  }
  return furthestIndex;
}

void rdp_support(curve const *original, double epsilon, int sidx, int eidx,
                 int *included, int *totalPoints) {

  if (sidx >= eidx) {
    return;
  }

  double d;
  int furthestIdx = furthestPoint(original, sidx, eidx, &d);
  if (furthestIdx != -1) {
    if (d < epsilon) {
      return;
    } else {
      included[furthestIdx] = 1;
      *totalPoints = *totalPoints + 1;
      rdp_support(original, epsilon, sidx, furthestIdx, included, totalPoints);
      rdp_support(original, epsilon, furthestIdx, eidx, included, totalPoints);
    }
  }
}

curve rdp(curve const *start, double epsilon) {

  int *included = calloc(start->length, sizeof(int));
  included[0] = 1;
  included[start->length - 1] = 1;

  int totalPoints = 2;

  rdp_support(start, epsilon, 0, start->length - 1, included, &totalPoints);

  point *result = malloc(sizeof(point) * totalPoints);

  int resultIdx = 0;
  for (int i = 0; i < start->length; i++) {
    if (included[i]) {
      assert(resultIdx < totalPoints);
      result[resultIdx].x = start->points[i].x;
      result[resultIdx].y = start->points[i].y;
      resultIdx++;
    }
  }

  free(included);

  curve v;
  v.points = result;
  v.length = totalPoints;

  return v;
}

void rdp_result_free(curve *c) { free(c->points); }

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
