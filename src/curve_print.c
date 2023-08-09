
#include "curve_print.h"
#include "point.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  double xmin = c->points[0].x;
  double xmax = c->points[0].x;
  double ymin = c->points[0].y;
  double ymax = c->points[0].y;
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

struct screen {
  int height, width;
  char **data;
};

static struct screen make_screen(int theight, int twidth) {
  char **data = malloc(sizeof(char *) * theight);

  if (data == NULL) {
    fprintf(stderr, "Out of memory!");
    abort();
  }

  for (int i = 0; i < theight; i++) {
    data[i] = malloc(sizeof(char) * twidth);
    if (data[i] == NULL) {
      fprintf(stderr, "Out of memory!");
      free(data);
      abort();
    }
    memset(data[i], '-', twidth);
  }

  struct screen s;
  s.data = data;
  s.height = theight;
  s.width = twidth;
  return s;
}

static void free_screen(struct screen s) {
  for (int i = 0; i < s.height; i++) {
    free(s.data[i]);
  }
  free(s.data);
}

void curve_print(curve const *c, struct curve_print_properties *prop) {
  bool x0 = false, y0 = false, xsym = false, ysym = false;
  if (prop != NULL) {
    x0 = prop->start_x_0;
    y0 = prop->start_y_0;
    xsym = prop->symmetricX;
    ysym = prop->symmetricY;
  }

  struct curve_extrema e;
  if (!get_curve_extrema(c, &e)) {
    fprintf(stderr, "No points in curve\n");
    return;
  }

  fix_bounds(&e.xmin, &e.xmax, xsym, x0);
  fix_bounds(&e.ymin, &e.ymax, ysym, y0);

  int theight, twidth;
  get_term_size(&theight, &twidth);

  struct screen s = make_screen(theight, twidth);

  for (int i = 0; i < c->length; i++) {
    point const *p = &c->points[i];
    int xchar = (int)map(p->x, e.xmin, e.xmax, 0.0, twidth - 1.0);
    int ychar = (int)map(p->y, e.ymin, e.ymax, 0.0, theight - 1.0);
    s.data[theight - ((int)ychar) - 1][(int)xchar] = 'X';
  }

  for (int i = 0; i < theight; i++) {
    for (int j = 0; j < twidth; j++) {
      putchar(s.data[i][j]);
    }
    putchar('\n');
  }

  free_screen(s);
}
