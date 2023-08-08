#include "curve.hpp"
#include "curve_print.hpp"
#include "point.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>

double radians(double deg) { return deg * (M_PI / 180); }

double degrees(double rad) { return rad * (180 / M_PI); }

template <std::floating_point T>
std::ostream &operator<<(std::ostream &right, point<T> const &left) {
  right << "(" << left.x << ", " << left.y << ")";
  return right;
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &os, curve<T> const &left) {
  os << "curve{";
  for (const auto &p : left.points()) {
    os << p << ", ";
  }
  os << "}";
  return os;
}

int main(int argc, char const *argv[]) {
  std::cout << "C++" << std::endl;

  double epsilon = 0.075;
  if (argc == 2) {
    printf("%s\n", argv[1]);
    epsilon = atof(argv[1]);
  }

  curve c;

  for (double x = -5; x < 5; x += 0.01) {
    c.addPoint(point{x, exp(-x) * cos(2 * M_PI * x)});
    // c.addPoint(point{x, exp(-x * cos(2 * M_PI * x))});
  }

  std::cout << "Beginning " << c.points().front() << " and "
            << c.points().back() << std::endl;

  std::cout << "start has length of " << c.length() << std::endl;

  curve r = c.rdp(epsilon);

  std::cout << "epsilon=" << epsilon << " result has " << r.length()
            << " points\n";

  std::cout << "After RDP " << r.points().front() << " and "
            << r.points().back() << std::endl;

  curve_print printer(false, false, false, false);
  printer.print(c);
  return 0;
}
