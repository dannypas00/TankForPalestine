#pragma once
namespace Quicksort
{
void quick_sort(std::vector<float>& original, std::vector<const float*>& sorted_tanks, UINT16 begin, UINT16 end);
int partition(std::vector<float>& original, std::vector<const float*>& result, UINT16 begin, UINT16 end);
void quick_sort_tanks(const std::vector<Tank>& original, std::vector<const Tank*>& sorted_tanks, UINT16 begin, UINT16 end);
int partition_tanks(const std::vector<Tank>& original, std::vector<const Tank*>& result, UINT16 begin, UINT16 end);
} // namespace Quicksort