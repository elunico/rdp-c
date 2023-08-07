#ifndef CURVE_HPP
#define CURVE_HPP

#include "point.hpp"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>

// adapted from processing/p5.js
template <std::floating_point T = double>
T map(T n, T start1, T stop1, T start2, T stop2) {
  T newval = (n - start1) / (stop1 - start1) * (stop2 - start2) + start2;
  return newval;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &left) {
  os << "[";
  for (int i = 0; i < left.size() - 1; i++) {
    os << left[i] << ", ";
  }
  os << left.back();
  os << "}";
  return os;
}

template <std::floating_point T = double> struct curve {

  [[nodiscard]] std::vector<point<T>> const &points() const noexcept {
    return points_;
  }

  [[nodiscard]] auto length() const noexcept { return points_.size(); }

  void addPoint(point<T> const &p) { points_.push_back(p); }

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

  void ttydraw() const {
    char *ctermheight = getenv("LINES");
    if (ctermheight == nullptr || *ctermheight == 0) {
      std::cerr << "Cannot get LINES" << std::endl;
      return;
    }

    char *ctermwidth = getenv("COLUMNS");
    if (ctermwidth == nullptr || *ctermwidth == 0) {
      std::cerr << "Cannot get columns" << std::endl;
      return;
    }

    int termheight = atoi(ctermheight);
    int termwidth = atoi(ctermwidth);

    std::vector<std::vector<char>> screen{};
    for (int i = 0; i < termheight; i++) {
      screen.emplace_back(std::vector<char>{});
      for (int j = 0; j < termwidth; j++) {
        screen.back().push_back('-');
      }
    }

    auto [ipminx, ipmaxx] = std::minmax_element(
        points_.begin(), points_.end(),
        [](point<T> const &a, point<T> const &b) { return a.x < b.x; });

    auto [ipminy, ipmaxy] = std::minmax_element(
        points_.begin(), points_.end(),
        [](point<T> const &a, point<T> const &b) { return a.y < b.y; });

    T minx = ipminx->x;
    T miny = ipminy->y;
    T maxx = ipmaxx->x;
    T maxy = ipmaxy->y;

    for (int i = 0; i < points_.size(); i++) {
      auto const &p = points_[i];
      T xchar = map(p.x, minx, maxx, 0.0, termwidth - 1.0);
      T ychar = map(p.y, miny, maxy, 0.0, termheight - 1.0);
      screen[termheight - ((int)ychar) - 1][(int)xchar] = 'X';
    }

    for (int i = 0; i < screen.size(); i++) {
      for (int j = 0; j < screen[i].size(); j++) {
        std::cout << screen[i][j];
      }
      std::cout << '\n';
    }
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
