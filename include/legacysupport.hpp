#ifndef LEGACY_SUPPORT_H
#define LEGACY_SUPPORT_H

#include <type_traits>

#if __cplusplus >= 202002L
#define FLOATING_POINT_CONCEPT std::floating_point
#else
#define FLOATING_POINT_CONCEPT typename
#endif

#endif
