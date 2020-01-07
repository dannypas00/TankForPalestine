#include "mergesort.h"
#include "precomp.h";

using namespace Tmpl8;

namespace Mergesort
{

void mergesort::merge(std::vector<Tank>& original, UINT16 l, UINT16 m, UINT16 r)
{
    UINT16 low = m - l + 1;
    UINT16 high = r - m;
    std::vector<Tank> leftVector, rightVector;
    leftVector.reserve(low);
    rightVector.reserve(high);

    for (UINT16 i = 0; i < low; i++)
    {
        leftVector.push_back(original[l + i]);
    }
    for (UINT16 j = 0; j < high; j++)
    {
        rightVector.push_back(original.at(m + 1 + j));
    }

    UINT16 i = 0;
    UINT16 j = 0;
    UINT16 k = l;

    while (i < low && j < high)
    {
        if (leftVector[i].health <= rightVector[j].health)
        {
            original[k] = leftVector[i];
            i++;
        }
        else
        {
            original[k] = rightVector[j];
            j++;
        }
        k++;
    }

    while (i < low)
    {
        original[k] = leftVector[i];
        i++;
        k++;
    }

    while (j < high)
    {
        original[k] = rightVector[j];
        j++;
        k++;
    }
}

void mergesort::sort(std::vector<Tank>& original, UINT16 l, UINT16 r)
{
    if (l < r)
    {
        UINT16 m = l + (r - l) / 2;

        sort(original, l, m);
        sort(original, m + 1, r);
        merge(original, l, m, r);
    }
}

}; // namespace Mergesort