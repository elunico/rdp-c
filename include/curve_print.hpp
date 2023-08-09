#ifndef CURVE_PRINT_HPP
#define CURVE_PRINT_HPP

#include "curve.hpp"
#include "point.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <type_traits>
#include <vector>

// adapted from processing/p5.js
template <std::floating_point T = double>
T map(T n, T start1, T stop1, T start2, T stop2) {
  auto delta = stop1 - start1 == 0 ? 1 : stop1 - start1;
  T newval = (n - start1) / (delta) * (stop2 - start2) + start2;
  return newval;
}

template <std::floating_point T> struct extrema {
  T minx;
  T miny;
  T maxx;
  T maxy;
};

class curve_print {

  bool start_x_0_;
  bool start_y_0_;
  bool symmetricX;
  bool symmetricY;
  int twidth;
  int theight;

  template <std::floating_point T>
  std::optional<extrema<T>> get_curve_extrema(curve<T> const &c) const {
    if (c.points().size() == 0) {
      return std::nullopt;
    }

    auto const &points_ = c.points();
    auto [ipminx, ipmaxx] = std::minmax_element(
        points_.begin(), points_.end(),
        [](point<T> const &a, point<T> const &b) { return a.x < b.x; });

    auto [ipminy, ipmaxy] = std::minmax_element(
        points_.begin(), points_.end(),
        [](point<T> const &a, point<T> const &b) { return a.y < b.y; });

    return std::optional{
        extrema<T>{ipminx->x, ipminy->y, ipmaxx->x, ipmaxy->y}};
  }

  template <std::floating_point T>
  std::pair<T, T> fix_bounds(T min, T max, bool sym, bool start0) const {
    if (start0 && min > 0)
      min = 0;

    if (sym && max >= 0) {
      if (-max < min)
        min = -max;
      else if (-min >= max)
        max = -min;
    }

    return std::make_pair(min, max);
  }

public:
  std::ostream &out = std::cout;

  curve_print(bool startX0, bool startY0, bool symmetricX, bool symmetricY,
              int twidth, int theight, std::ostream &out);

  curve_print(bool startX0, bool startY0, bool symmetricX, bool symmetricY,
              int twidth, int theight);

  curve_print(bool startX0, bool startY0, bool symmetricX, bool symmetricY);

  curve_print();

  curve_print(curve_print &) = default;

  curve_print(curve_print &&) = default;

  template <std::floating_point T> void print(curve<T> const &c) const {
    std::vector<std::vector<char>> screen{};
    for (int i = 0; i < theight; i++) {
      screen.emplace_back();
      for (int j = 0; j < twidth; j++) {
        screen.back().push_back('-');
      }
    }
    auto const &points_ = c.points();

    std::optional<extrema<T>> oe = get_curve_extrema(c);
    if (!oe.has_value()) {
      out << "No points in curve\n";
      return;
    }
    extrema<T> e = *oe;

    auto [minx, maxx] = fix_bounds(e.minx, e.maxx, symmetricX, start_x_0_);
    auto [miny, maxy] = fix_bounds(e.miny, e.maxy, symmetricY, start_y_0_);

    for (int i = 0; i < points_.size(); i++) {
      auto const &p = points_[i];
      T xchar = map(p.x, minx, maxx, 0.0, twidth - 1.0);
      T ychar = map(p.y, miny, maxy, 0.0, theight - 1.0);
      screen[theight - ((int)ychar) - 1][(int)xchar] = 'X';
    }

    for (int i = 0; i < screen.size(); i++) {
      for (int j = 0; j < screen[i].size(); j++) {
        out << screen[i][j];
      }
      out << '\n';
    }
  }
};

#endif
