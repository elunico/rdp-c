#include "curve_print.hpp"

curve_print::curve_print(bool startX0, bool startY0, bool symmetricX,
                         bool symmetricY, int twidth, int theight,
                         std::ostream &out)
    : start_x_0_(startX0), start_y_0_(startY0), symmetricX(symmetricX),
      symmetricY(symmetricY), twidth(twidth), theight(theight), out(out) {}

curve_print::curve_print(bool startX0, bool startY0, bool symmetricX,
                         bool symmetricY, int twidth, int theight)
    : curve_print(startX0, startY0, symmetricX, symmetricY, twidth, theight,
                  std::cout) {}

curve_print::curve_print() : curve_print(false, false, false, true) {}

curve_print::curve_print(bool startX0, bool startY0, bool symmetricX,
                         bool symmetricY)
    : start_x_0_(startX0), start_y_0_(startY0), symmetricX(symmetricX),
      symmetricY(symmetricY) {
  char *ctermheight = getenv("LINES");
  if (ctermheight == nullptr || *ctermheight == 0) {
    throw std::invalid_argument("environment does not contain LINES");
  }

  char *ctermwidth = getenv("COLUMNS");
  if (ctermwidth == nullptr || *ctermwidth == 0) {
    throw std::invalid_argument("environment does not contain COLUMNS");
  }

  int termheight = static_cast<int>(strtol(ctermheight, nullptr, 10));
  int termwidth = static_cast<int>(strtol(ctermwidth, nullptr, 10));

  curve_print::theight = termheight;
  curve_print::twidth = termwidth;
}
