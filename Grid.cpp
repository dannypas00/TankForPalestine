#include "precomp.h"

std::vector<std::vector<vector<Tank*>>> GridVector;
UINT16 cellSize;
Grid::Grid::Grid(int width, int height, UINT16 cellSize)
{
    std::vector<std::vector<vector<Tank*>>> xVector;
    for (int x = (width / cellSize) - 1; x >= 0; x--)
    {
        std::vector<std::vector<Tank*>> yVector;
        for (int y = (height / cellSize) - 1; y >= 0; y--)
        {
            yVector.at(y) = std::vector<Tank*>();
        }
        xVector.at(x) = yVector;
    }
    GridVector = xVector;
}

void Grid::Grid::FillGrid(std::vector<Tank*>& tanks)
{
    for (Tank* tank : tanks)
    {
        vec2 pos = GetGridPoint(tank->position);
        float x = pos.x;
        float y = pos.y;
        GridVector.at(x).at(y).emplace_back(tank);
    }
}

vec2 Grid::Grid::GetGridPoint(vec2 position)
{
    return vec2(floor(position.x / cellSize), floor(position.y / cellSize));
}