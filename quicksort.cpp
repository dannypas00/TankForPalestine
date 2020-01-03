#include "precomp.h" // include (only) this in every .cpp file
#include "quicksort.h"
using namespace Tmpl8;

namespace Quicksort
{
//UNDER DEVELOPMENT
void quick_sort(std::vector<float>& original, std::vector<const float*>& result, UINT16 begin, UINT16 end)
{
    const UINT16 amount = end - begin;
    result.reserve(amount);
    result.emplace_back(&original.at(begin));

    UINT16 temp;
    if (begin < end)
    {
        temp = partition(original, begin, end);
        quick_sort(original, result, begin, temp - 1);  //Opportunity for concurrency
        quick_sort(original, result, temp + 1, end);
    }
}

//https://www.thecrazyprogrammer.com/2016/11/program-for-quick-sort-c.html
int partition(std::vector<float>& original, UINT16 begin, UINT16 end)
{
    UINT16 i, j;
    float current = original[begin];
    float temp = original[0];
    i = begin;
    j = end + 1;

    do
    {
        do
        {
            i++;
        } while (original[i] < current && i <= end);

        do
        {
            j--;
        } while (current < original.at(j));

        if (i < j)
        {
            temp = original.at(i);
            original[i] = original.at(j);
            original[j] = temp;
        }
    } while (i < j);

    original[begin] = original.at(j);
    original.at(j) = current;

    return (j);
}

void quick_sort_tanks(std::vector<Tank> original, UINT16 begin, UINT16 end)
{
    UINT16 temp;
    if (begin < end)
    {
        temp = partition_tanks(original, begin, end);
        quick_sort_tanks(original, begin, temp - 1);    //There is an index out of bounds error regarding the start and end temp values.
        quick_sort_tanks(original, temp + 1, end);
    }
}

int partition_tanks(std::vector<Tank> original, UINT16 begin, UINT16 end)
{
    UINT16 i, j;
    std::cout << "Begin: " << begin << std::endl;
    std::cout << "End: " << end << std::endl;
    std::cout << "Length: " << original.size() << std::endl;
    Tank current = original.front();
    Tank temp = original.front();
    i = begin;
    j = end + 1;

    do
    {
        do
        {
            i++;
        } while (original[i].health < current.health && i <= end);

        do
        {
            j--;
        } while (current.health < original[j].health);

        if (i < j)
        {
            temp = original[i];
            original[i] = original[j];
            original[j] = temp;
        }
    } while (i < j);

    original[begin] = original[j];
    original[j] = current;

    return (j);
}
} // namespace Quicksort