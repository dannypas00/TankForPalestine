#include "mergesort.h"
#include "precomp.h";

using namespace Tmpl8;

namespace Mergesort
{

void mergesort::mergeHealth(std::vector<Tank>& original, UINT16 l, UINT16 m, UINT16 r)
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

void mergesort::mergeX(std::vector<Tank*>& original, UINT16 l, UINT16 m, UINT16 r)
{
    UINT16 low = m - l + 1;
    UINT16 high = r - m;
    std::vector<Tank*> leftVector, rightVector;
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
        if (leftVector[i] -> position.x <= rightVector[j] -> position.x)
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

void mergesort::merge(std::vector<double>& original, UINT16 l, UINT16 m, UINT16 r)
{
    UINT16 low = m - l + 1;
    UINT16 high = r - m;
    std::vector<double> leftVector, rightVector;
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
        if (leftVector[i] <= rightVector[j])
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

void mergesort::sortHealth(std::vector<Tank>& original, UINT16 l, UINT16 r)
{
    if (l < r)
    {
        UINT16 m = l + (r - l) / 2;

        sortHealth(original, l, m);
        sortHealth(original, m + 1, r);
        mergeHealth(original, l, m, r);
    }
}

void mergesort::sortX(std::vector<Tank>& original, UINT16 l, UINT16 r)
{
    if (l < r)
    {
        UINT16 m = l + (r - l) / 2;

        sortX(original, l, m);
        sortX(original, m + 1, r);
        mergeX(original, l, m, r);
    }
}

void mergesort::sort(std::vector<double>& original, UINT16 l, UINT16 r)
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