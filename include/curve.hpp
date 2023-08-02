#ifndef CURVE_HPP
#define CURVE_HPP

#include "point.hpp"
#include <tuple>
#include <type_traits>
#include <vector>

template <std::floating_point T> struct curve {

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
