
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
