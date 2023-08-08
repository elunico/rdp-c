
#include "curve_print.h"

// adapted from processing/p5.js
double map(double n, double start1, double stop1, double start2, double stop2) {
  double delta = stop1 - start1 == 0 ? 1 : stop1 - start1;
  double newval = (n - start1) / (delta) * (stop2 - start2) + start2;
  return newval;
}

void get_term_size(int *h, int *w) {
  char *ctermheight = getenv("LINES");
  if (ctermheight == NULL || *ctermheight == 0) {
    fprintf(stderr, "environment does not contain LINES\n");
    abort();
  }

  char *ctermwidth = getenv("COLUMNS");
  if (ctermwidth == NULL || *ctermwidth == 0) {
    fprintf(stderr, "environment does not contain COLUMNS\n");
    abort();
  }

  int termheight = (int)(strtol(ctermheight, NULL, 10));
  int termwidth = (int)(strtol(ctermwidth, NULL, 10));

  *h = termheight;
  *w = termwidth;
}

bool get_curve_extrema(curve const *c, struct curve_extrema *result) {
  if (result == NULL) {
    fprintf(stderr, "Do not pass null to get_curve_extrema\n");
    abort();
  }

  if (c->length == 0)
    return false;

  point *points = c->points;

  double xmin, xmax, ymin, ymax;
  for (int i = 1; i < c->length; i++) {
    if (c->points[i].x > xmax)
      xmax = c->points[i].x;

    if (c->points[i].x < xmin)
      xmin = c->points[i].x;

    if (c->points[i].y > ymax)
      ymax = c->points[i].y;

    if (c->points[i].y < ymin)
      ymin = c->points[i].y;
  }

  result->xmax = xmax;
  result->xmin = xmin;
  result->ymax = ymax;
  result->ymin = ymin;
  return true;
}

void fix_bounds(double *min, double *max, bool sym, bool start0) {
  if (start0 && *min > 0)
    *min = 0;

  if (sym && *max >= 0) {
    if (-*max < *min)
      *min = -*max;
    else if (-*min >= *max)
      *max = -*min;
  }
}

void print(curve const *c) {
  int theight, twidth;
  get_term_size(&theight, &twidth);

  char **screen = malloc(sizeof(char *) * theight);
  for (int i = 0; i < theight; i++) {
    screen[i] = malloc(sizeof(char) * twidth);
    memset(screen[i], '-', twidth);
  }

  point *points_ = c->points;

  struct curve_extrema e;

  if (!get_curve_extrema(c, &e)) {
    fprintf(stderr, "No points in curve\n");
    return;
  }

  fix_bounds(&e.xmin, &e.xmax, false, false); // symmetricX, start_x_0_);
  fix_bounds(&e.ymin, &e.ymax, false, false); // symmetricY, start_y_0_);

  for (int i = 0; i < c->length; i++) {
    point const *p = &c->points[i];
    int xchar = (int)map(p->x, e.xmin, e.xmax, 0.0, twidth - 1.0);
    int ychar = (int)map(p->y, e.ymin, e.ymax, 0.0, theight - 1.0);
    screen[theight - ((int)ychar) - 1][(int)xchar] = 'X';
  }

  for (int i = 0; i < theight; i++) {
    for (int j = 0; j < twidth; j++) {
      // out << screen[i][j];
      putchar(screen[i][j]);
    }
    // out << '\n';
    putchar('\n');
  }
}
