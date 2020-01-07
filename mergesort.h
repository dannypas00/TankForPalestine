#pragma once

//#include "Tank.h"

namespace Mergesort
{
class mergesort
{
  public:
    static void sort(std::vector<Tank>& original, UINT16 l, UINT16 r);
    static void sort(std::vector<double>& original, UINT16 l, UINT16 r);
    static void merge(std::vector<Tank>& original, UINT16 l, UINT16 m, UINT16 r);
    static void merge(std::vector<double>& original, UINT16 l, UINT16 m, UINT16 r);
};
} // namespace Mergesort