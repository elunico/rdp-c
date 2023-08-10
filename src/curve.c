#include "curve.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int furthestPoint(curve const *inCurve, int start, int end, double *distance) {

#ifdef DEBUG
  if (distance == NULL) {
    fprintf(stderr, "furthestPoint(curve const*, int, int, double* distance) "
                    "distance cannot be null\n");
    abort();
  }
#endif

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

static void rdp_support(curve const *original, double epsilon, int sidx,
                        int eidx, int *included, int *totalPoints) {

  if (sidx >= eidx)
    return;

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

curve *rdp(curve const *start, double epsilon) {

#ifdef DEBUG
  if (start == NULL) {
    fprintf(stderr, "Do not pass null to rdp()\n");
    abort();
  }
#endif

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

  curve *v = malloc(sizeof(*v));
  v->points = result;
  v->length = totalPoints;

  return v;
}

void rdp_result_free(curve *c) {
  free(c->points);
  free(c);
}

void curve_quadratic_free(curve *c) {
  free(c->points);
  free(c);
}
void curve_linear_free(curve *c) {
  free(c->points);
  free(c);
}

static double quadratic(double a, double b, double c, double x) {
  return a * pow(x, 2) + b * c + c;
}

static int count_between(double start, double end, double delta) {
  double c = fabs(end - start) / delta;
  int q = (int)c;
  double difference = c - q;
  printf("c is %d difference is %f and q is %d\n", (int)c, difference, q);
  return ((int)c + 1) + (fabs(difference) != 0);
}

curve *curve_from_quadratic(double a, double b, double c, double xstart,
                            double xend, double delta) {
  curve *result = malloc(sizeof(*result));
  int count = count_between(xstart, xend, delta) + 1;
  printf("count is %d\n", count);
  result->points = malloc(sizeof(point) * count);
  result->length = count;
  int index = 0;
  for (double x = xstart; x < xend; x += delta) {
    result->points[index].x = x;
    result->points[index++].y = quadratic(a, b, c, x);
  }
  result->points[result->length - 1].x = xend;
  result->points[result->length - 1].y = quadratic(a, b, c, xend);
  result->length++;
  return result;
}

static double linear_eq(double x1, double y1, double slope, double x) {
  return slope * x - slope * x1 + y1;
}

curve *curve_from_line(double x1, double y1, double x2, double y2,
                       double delta) {
  double slope = (y2 - y1) / (x2 - x1);
  curve *result = malloc(sizeof(*result));
  // int count = (int)((fabs(y1 - y2) / delta) + 1);
  int count = count_between(y2, y1, delta) + 1;
  result->points = malloc(sizeof(point) * count);
  result->length = count;
  int index = 0;
  if (isinf(slope)) {
    if (y2 < y1) {
      for (double start = y1; start > y2; start -= delta) {
        result->points[index].x = x1;
        result->points[index++].y = start;
      }
      result->points[result->length - 1].x = x2;
      result->points[result->length - 1].y = linear_eq(x1, y1, slope, x2);
      result->length++;
    } else {
      for (double start = y1; start < y2; start += delta) {
        result->points[index].x = x1;
        result->points[index++].y = start;
      }
      result->points[result->length - 1].x = x2;
      result->points[result->length - 1].y = linear_eq(x1, y1, slope, x2);
      result->length++;
    }
  } else {
    for (double start = x1; start < x2; start += delta) {
      double y = linear_eq(x1, y1, slope, start);
      result->points[index].x = start;
      result->points[index++].y = y;
    }
    result->points[result->length - 1].x = x2;
    result->points[result->length - 1].y = linear_eq(x1, y1, slope, x2);
    result->length++;
  }
  return result;
}

curve *curve_construct(double startX, double endX, double delta,
                       double (*f)(double)) {
  curve *result = malloc(sizeof(*result));
  result->length = 0;
  // int count = (int)((fabs(endX - startX) / delta) + 1);
  int count = count_between(startX, endX, delta) + 1;
  result->points = malloc(sizeof(point) * count);
  int index = 0;
  while (startX < endX) {
    result->points[index].x = startX;
    result->points[index++].y = f(startX);
    startX += delta;
    result->length++;
  }
  fprintf(stderr, "result length is %d and count is %d\n", result->length,
          count);
  // assert((result->length == (count - 1)) || (result->length == (count - 2)));
  result->points[result->length - 1].x = endX;
  result->points[result->length - 1].y = f(endX);
  result->length++;
  return result;
}

void curve_construct_free(curve *c) {
  free(c->points);
  free(c);
}
