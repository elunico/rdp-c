#ifndef CURVE_PRINT_H
#define CURVE_PRINT_H

#include "curve.h"
#include <stdbool.h>

/**
 * start_x_0 - start x values at 0. ignored if the minimum x value in the curve
 * is < 0 start_y_0 - start y values at 0. ignored if the minimum y value in the
 * curve is < 0 symmetricX - print the x values so that the max and min are
 * equidistant symmetricY - print the y values so that the max and min are
 * equidistant
 */
struct curve_print_properties {
  bool start_x_0, start_y_0, symmetricX, symmetricY;
};

/**
 * Represents the smallest and largest x and y values in the curve
 * Note that this does not contain points. The smallest and largest
 * values of each coordinate are independently found. Therefore,
 * for example, the point (xmin, ymin) may not exist in the curve,
 * but there must be at least 1 point (xmin, _) and at least 1 point
 * (_, ymin) where _ can be any value.
 */
struct curve_extrema {
  double xmin, xmax, ymin, ymax;
};

/**
 * this function takes a value _n_ in the range _start1_ to _stop1_ and places
 * it in an equivalent position in the range _start2_ to _stop2_.
 */
double map(double n, double start1, double stop1, double start2, double stop2);

/**
 * This function retrieves LINES and COLUMNS from the environment and parses
 * them as ints, storing the result in _h_ and _w_ respectively.
 *
 * NULL can be passed in _h_ and/or _w_ without ill effects, but the function
 * will not do anything if it does
 *
 * returns: true if LINES and COLUMNS were successfully read, converted, and
 * stored in _h_ and _w_ and false otherwise.
 */
bool get_term_size(int *h, int *w);

/**
 * fills in the struct curve_extrema pointed to by _result_ with the
 * smallest and largest x and y values found in curve _c_.
 * Note that the smallest x and y may come from two distinct points.
 * This function finds the smallest and largest of each coordinate
 * *NOT* the coordinate with the smallest/largest x and y.
 * Do not pass NULL to any arguments
 *
 * returns: true on success and false if the curve has no points
 */
bool get_curve_extrema(curve const *c, struct curve_extrema *result);

/**
 * changes the values points to my _min_ and _max_ depending on whether
 * the values of min and max should be symmetric and whether the range
 * should start at 0. Note that if *min < 0, start0 is ignored.
 * Do not pass NULL to any arguments
 */
void fix_bounds(double *min, double *max, bool sym, bool start0);

/**
 * prints the curve _c_ to the console using the options specified by
 * _props_. To use defaults you can pass _props_ as NULL.
 */
void curve_print(curve const *c, struct curve_print_properties *props);

#endif
