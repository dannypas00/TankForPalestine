#pragma once

//#include "Tank.h"

namespace Mergesort
{
class mergesort
{
  public:
    static void sort(std::vector<double>& original, UINT16 l, UINT16 r);
    static void sortHealth(std::vector<Tank>& original, UINT16 l, UINT16 r);
    static void sortX(std::vector<Tank>& original, UINT16 l, UINT16 r);
    static void merge(std::vector<double>& original, UINT16 l, UINT16 m, UINT16 r);
    static void mergeHealth(std::vector<Tank>& original, UINT16 l, UINT16 m, UINT16 r);
    static void mergeX(std::vector<Tank>& original, UINT16 l, UINT16 m, UINT16 r);
};
} // namespace Mergesort