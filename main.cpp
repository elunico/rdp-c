#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

struct point {
  double x, y;

  point(double x, double y) : x(x), y(y) {}
  point() : x(0), y(0) {}

  double dist(point const &other) const {
    double dx = x - other.x;
    double dy = y - other.y;

    return std::sqrt((dx * dx) + (dy * dy));
  }

  double dot(point const &b) const {
    double result = x * b.x + y * b.y;
    return result;
  }

  double mag() const { return std::sqrt((x * x) + (y * y)); }

  void normalize() {
    double m = mag();
    assert(m != INFINITY);
    assert(m != 0);
    x = x / m;
    y = y / m;
  }

  point scalarProjection(point const &a, point const &b) const {
    point ap, ab;

    ap.x = x - a.x;
    ap.y = y - a.y;

    ab.x = b.x - a.x;
    ab.y = b.y - a.y;

    ab.normalize();

    double sp = ap.dot(ab);
    ab.x *= sp;
    ab.y *= sp;

    ab.x += a.x;
    ab.y += a.y;

    return ab;
  }

  double p2ldist(point const &l1, point const &l2) const {
    point norm = scalarProjection(l1, l2);
    return dist(norm);
  }
};

struct curve {

  std::vector<point const> const &points() const noexcept { return points_; }

  int length() const { return points_.size(); }

  void addPoint(point const &p) { points_.push_back(p); }

  std::tuple<int, double> furthestPoint(int start, int end) const {

    int furthestIndex = -1;
    double recordDist = 0;

    point const &s = points_[start];
    point const &e = points_[end];
    for (int i = start + 1; i < end; i++) {
      double t = points_[i].p2ldist(s, e);
      if (t > recordDist) {
        furthestIndex = i;
        recordDist = t;
      }
    }
    return std::make_tuple(furthestIndex, recordDist);
  }

  curve rdp(double epsilon) {

    curve result;

    result.addPoint(points_.front());
    rdp_support(epsilon, 0, length() - 1, result);
    result.addPoint(points_.back());

    return result;
  }

private:
  std::vector<point const> points_;

  void rdp_support(double epsilon, int sidx, int eidx, curve &result) {

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

std::ostream &operator<<(std::ostream &right, point const &left) {
  right << "(" << left.x << ", " << left.y << ")";
  return right;
}

int main(int argc, char const *argv[]) {
  std::cout << "C++" << std::endl;

  double epsilon = 0.075;
  if (argc == 2) {
    printf("%s\n", argv[1]);
    epsilon = atof(argv[1]);
  }

  curve c;

  int index = 0;
  for (double d = 0; d < 5; d += 0.01) {
    c.addPoint(point{d, exp(-d * cos(2 * M_PI * d))});
  }

  std::cout << "Beginning " << c.points().front() << " and "
            << c.points().back() << std::endl;

  std::cout << "start has length of " << c.length() << std::endl;

  curve r = c.rdp(epsilon);

  std::cout << "epsilon=" << epsilon << " result has " << r.length()
            << " points\n";

  std::cout << "After RDP " << r.points().front() << " and "
            << r.points().back() << std::endl;

  return 0;
}
