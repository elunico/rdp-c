#ifndef CURVE_HPP
#define CURVE_HPP

#include "legacysupport.hpp"
#include "point.hpp"
#include <tuple>
#include <type_traits>
#include <vector>

template <FLOATING_POINT_CONCEPT T = double> struct sortXs {
  bool operator()(auto a, auto b) { return a.x < b.x; }
};

/**
 * Class representing a curve composed of discrete points
 * Curve can be simplified using the Ramer-Douglas-Peuker
 * algorithm.
 *
 * Implementation note: delta is treated as a maximum.
 * points may be closer together than delta but will not
 * be further apart than delta
 */
template <FLOATING_POINT_CONCEPT T = double> struct curve {

  /**
   * Create a curve between startX and endX with a delta
   * of at most delta and using the function f to get y values
   */
  static curve<T> construct(T startX, T endX, T delta, auto f) {
    curve<T> result;
    for (; startX < endX; startX += delta)
      result.addPoint(startX, f(startX));
    result.addPoint(endX, f(endX));
    return result;
  }

  /**
   * Construct a quadratic curve between xstart and xend
   * The form is y=ax^2 + bx + c
   */
  static curve<T> quadratic(T a, T b, T c, T xstart, T xend, T delta = 0.01) {
    auto equation = [&](T x) { return a * std::pow(x, 2) + b * x + c; };
    curve<T> result;
    for (double x = xstart; x < xend; x += delta)
      result.addPoint(x, equation(x));
    result.addPoint(xend, f(xend));
    return result;
  }

  /**
   * Construct a line between the two points (x1, y1) and (x2, y2) with
   * at most delta space between x-coordinates of the points
   */
  static curve<T> line_between(T x1, T y1, T x2, T y2, T delta = 0.01) {
    T slope = (y2 - y1) / (x2 - x1);
    curve<T> result;
    if (isinf(slope)) {
      if (y2 < y1) {
        for (T start = y1; start > y2; start -= delta) {
          result.addPoint(point<T>{x1, start});
        }
      } else {
        for (T start = y1; start < y2; start += delta) {
          result.addPoint(point<T>{x1, start});
        }
      }
      result.addPoint(point<T>{x2, y2});
    } else {
      auto equation = [&](T x) { return slope * x - slope * x1 + y1; };
      for (T start = x1; start < x2; start += delta) {
        T y = equation(start);
        result.addPoint(point{start, y});
      }
      result.addPoint(x2, equation(x2));
    }
    return result;
  }

  [[nodiscard]] std::vector<point<T>> const &points() const noexcept {
    return points_;
  }

  [[nodiscard]] auto length() const noexcept { return points_.size(); }

  /**
   * Add a point to the curve. Note that this method does no sorting
   * and points in the curve should be sorted by their x coordinate.
   *
   * This method exists as an optimization so that if you are adding
   * points in order, you do not have to pay the sort cost. It can
   * also be used to add all points quickly then sortPoints() can be
   * called to sort the existing points by x
   */
  void addPoint(point<T> const &p) { points_.push_back(p); }

  /**
   * Add a point to the curve. Note that this method does no sorting
   * and points in the curve should be sorted by their x coordinate.
   *
   * This method exists as an optimization so that if you are adding
   * points in order, you do not have to pay the sort cost. It can
   * also be used to add all points quickly then sortPoints() can be
   * called to sort the existing points by x
   */
  void addPoint(T x, T y) { points_.emplace_back(x, y); }

  /**
   * Add a point in the correct position.
   *
   * DO NOT CALL THIS METHOD IF THE CURVE'S POINTS ARE NOT ALREADY SORTED
   * This method uses a linear search to place the new point, and
   * requires the points to already be sorted.
   */
  void addPointSorted(point<T> const &p) {
    for (auto it = points().begin(); it < points().end(); it++) {
      if (p.x > it->x) {
        points_.insert(it, p);
        return;
      }
    }
    points_.push_back(p);
  }

  void sortPoints() {
    std::sort(std::begin(points_), std::end(points_), sortXs{});
  }

  [[nodiscard]] auto furthestPoint(int start, int end) const {

    int furthestIndex = -1;
    T recordDist = 0;

    point<T> const &s = points_[start];
    point<T> const &e = points_[end];
    for (int i = start + 1; i < end; i++) {
      T t = points_[i].p2ldist(s, e);
      if (t > recordDist) {
        furthestIndex = i;
        recordDist = t;
      }
    }
    return std::make_tuple(furthestIndex, recordDist);
  }

  curve rdp(double epsilon) const {
    curve result;

    result.addPoint(points_.front());
    rdp_support(epsilon, 0, length() - 1, result);
    result.addPoint(points_.back());

    return result;
  }

private:
  std::vector<point<T>> points_;

  void rdp_support(double epsilon, int sidx, int eidx, curve &result) const {

    if (sidx >= eidx)
      return;

    auto [furthestIdx, d] = furthestPoint(sidx, eidx);
    if (furthestIdx != -1) {
      if (d < epsilon) {
        return;
      } else {
        rdp_support(epsilon, sidx, furthestIdx, result);
        result.points_.push_back(points_[furthestIdx]);
        rdp_support(epsilon, furthestIdx, eidx, result);
      }
    }
  }
};

#endif
