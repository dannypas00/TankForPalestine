#pragma once
namespace Cell
{
class Cell
{
  public:
    Cell(float x, float y);
    vec2 GetPosition();
    std::vector<Tank*>& GetTanks();
    void AddTank(Tank* tank);
    vec2 position;
    std::vector<Tank*>& tanks;
};
} // namespace Cell