#pragma once
namespace Grid
{
class Grid
{
  public:
    Grid(int width, int height, UINT16 cellSize);
    void FillGrid(std::vector<Tank*>& tanks);
    vec2 GetGridPoint(vec2 position);
};
} // namespace Grid