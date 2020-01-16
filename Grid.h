#pragma once
namespace Grid
{
class Grid
{
  public:
    UINT16 cellSize;
    std::vector<std::vector<vector<Tank*>>> GridVector;
    Grid(int width, int height, UINT16 cellSize);
    void FillGrid(Tank* tank);
    vec2 GetGridPoint(vec2 position);
    Tank* FindNearestNeighbor(Tank* tank);
};
} // namespace Grid