#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

double radians(double deg) { return deg * (M_PI / 180); }

double degrees(double rad) { return rad * (180 / M_PI); }

struct point {
  double x, y;

  point(double x, double y) : x(x), y(y) {}

  point() : x(0), y(0) {}

  static point fromangle(double rads) {
    double x = cos(rads);
    double y = sin(rads);
    return point{x, y};
  }

  static point fromangle(double rads, double magnitude) {
    return point::fromangle(rads) * magnitude;
  }

  [[nodiscard]] point copy() const { return point{x, y}; }

  [[nodiscard]] double dist(point const &other) const {
    double dx = x - other.x;
    double dy = y - other.y;

    return std::sqrt((dx * dx) + (dy * dy));
  }

  [[nodiscard]] double dot(point const &b) const { return x * b.x + y * b.y; }

  [[nodiscard]] double mag() const { return std::sqrt(dot(*this)); }

  [[nodiscard]] double heading() const {
    point c = copy();
    c.normalize();
    return atan2(c.y, c.x);
  }

  void normalize() {
    double m = mag();
    assert(m != INFINITY);
    if (m == 0)
      return;
    x = x / m;
    y = y / m;
  }

  point operator/(double rhs) const {
    auto p = copy();
    p.x /= rhs;
    p.y /= rhs;
    return p;
  }

  point operator*(double rhs) const {
    auto p = copy();
    p.x *= rhs;
    p.y *= rhs;
    return p;
  }

  point operator-(point const &other) const {
    point r;
    r.x = x - other.x;
    r.y = y - other.y;
    return r;
  }

  point operator+(point const &other) const {
    point r;
    r.x = x + other.x;
    r.y = y + other.y;
    return r;
  }

  [[nodiscard]] point scalarProjection(point const &a, point const &b) const {
    point ap = *this - a;
    point ab = b - a;

    ab.normalize();

    double sp = ap.dot(ab);

    return (ab * sp) + a;
  }

  [[nodiscard]] double p2ldist(point const &l1, point const &l2) const {
    point norm = scalarProjection(l1, l2);
    return dist(norm);
  }
};

struct curve {

  [[nodiscard]] std::vector<point> const &points() const noexcept {
    return points_;
  }

  [[nodiscard]] std::size_t length() const { return points_.size(); }

  void addPoint(point const &p) { points_.push_back(p); }

  [[nodiscard]] std::tuple<int, double> furthestPoint(int start,
                                                      int end) const {

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
  std::vector<point> points_;

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

  double d = 0;
  while (d < 5) {
    c.addPoint(point{d, exp(-d * cos(2 * M_PI * d))});
    d += 5;
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
