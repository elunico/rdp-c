#include "curve.h"
#include <assert.h>
#include <stdlib.h>

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
