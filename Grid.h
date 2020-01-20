#pragma once
namespace Tmpl8
{
class Grid
{
  private:
    float rocket_radius = 10.0F;
    float tank_radius = 12.0F;
    static const UINT16 cellSize = 12.0F;
  public:
    std::vector<std::vector<vector<Tank*>>> GridVector;
    const vec2 down{0, -1}, left {-1, 0}, up {0, 1}, right {1, 0};
    Grid(int width, int height);
    void FillGrid(Tank* tank);
    static vec2 GetGridPoint(vec2 position);
    std::vector<std::vector<Tank*>*> CollisionCheck(Rocket& rocket);
    Tank* FindNearestNeighbor(Tank* tank);
    void Grid::Grid::ClearGrid();
};
} // namespace Grid