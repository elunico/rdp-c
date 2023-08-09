#ifndef POINT_HPP
#define POINT_HPP

#include "legacysupport.hpp"
#include <cassert>
#include <cmath>
#include <tuple>
#include <type_traits>
#include <vector>

template <FLOATING_POINT_CONCEPT T = double> struct point {
  T x, y;

  point(T x, T y) noexcept : x(x), y(y) {}

  point() noexcept : x(0), y(0) {}

  static point<T> fromangle(double rads) {
    double x = cos(rads);
    double y = sin(rads);
    return point{x, y};
  }

  static point<T> fromangle(double rads, double magnitude) {
    return point::fromangle(rads) * magnitude;
  }

  [[nodiscard]] point<T> copy() const { return point{x, y}; }

  [[nodiscard]] T dist(point<T> const &other) const {
    T dx = x - other.x;
    T dy = y - other.y;

    return std::sqrt((dx * dx) + (dy * dy));
  }

  [[nodiscard]] T dot(point<T> const &b) const { return x * b.x + y * b.y; }

  [[nodiscard]] T mag() const { return std::sqrt(dot(*this)); }

  [[nodiscard]] T heading() const {
    point<T> c = copy();
    c.normalize();
    return std::atan2(c.y, c.x);
  }

  void normalize() {
    T m = mag();
    assert(m != INFINITY);
    if (m == 0)
      return;
    x = x / m;
    y = y / m;
  }

  point<T> operator/(double rhs) const {
    auto p = copy();
    p.x /= rhs;
    p.y /= rhs;
    return p;
  }

  point<T> operator*(double rhs) const {
    auto p = copy();
    p.x *= rhs;
    p.y *= rhs;
    return p;
  }

  point<T> operator-(point<T> const &other) const {
    point<T> r;
    r.x = x - other.x;
    r.y = y - other.y;
    return r;
  }

  point<T> operator+(point<T> const &other) const {
    point<T> r;
    r.x = x + other.x;
    r.y = y + other.y;
    return r;
  }

  [[nodiscard]] point<T> scalarProjection(point<T> const &a,
                                          point<T> const &b) const {
    point<T> ap = *this - a;
    point<T> ab = b - a;

    ab.normalize();

    T sp = ap.dot(ab);

    return (ab * sp) + a;
  }

  [[nodiscard]] T p2ldist(point<T> const &l1, point<T> const &l2) const {
    point<T> norm = scalarProjection(l1, l2);
    return dist(norm);
  }
};

#endif
