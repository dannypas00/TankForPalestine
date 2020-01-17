#pragma once
namespace Grid
{
class Grid
{
  public:
    UINT16 cellSize;
    std::vector<std::vector<vector<Tank*>>> GridVector;
    const vec2 down{0, -1}, left {-1, 0}, up {0, 1}, right {1, 0};
    Grid(int width, int height, UINT16 cellSize);
    void FillGrid(Tank* tank);
    vec2 GetGridPoint(vec2 position);
    Tank* FindNearestNeighbor(Tank* tank);
};
} // namespace Grid