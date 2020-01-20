#include "precomp.h"

class Cell
{
    vec2 position;
    std::vector<Tank*> tanks = std::vector<Tank*>();
    Cell(float x, float y)
    {
        position = {x,
                    y};
    }
    vec2 GetPosition()
    {
        return position;
    }
    std::vector<Tank*> GetTanks()
    {
        return tanks;
    }
    void AddTank(Tank* tank)
    {
        tanks.emplace_back(tank);
    }
};