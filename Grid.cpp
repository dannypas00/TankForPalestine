#include "precomp.h"

std::vector<std::vector<vector<Tank*>>> GridVector;
UINT16 cellSize;
Grid::Grid::Grid(int width, int height, UINT16 cSize)
{
    cellSize = cSize;
    std::vector<std::vector<vector<Tank*>>> xVector;
    for (int x = (width / cSize) - 99; x >= 100; x--)
    {
        std::vector<std::vector<Tank*>> yVector;
        for (int y = (height / cSize) - 99; y >= 100; y--)
        {
            yVector.push_back(std::vector<Tank*>());
        }
        xVector.push_back(yVector);
    }
    GridVector = xVector;
}

void Grid::Grid::FillGrid(Tank* tank)
{
    vec2 pos = GetGridPoint(tank->position);
    float x = pos.x - 1;
    float y = pos.y - 1;
    GridVector.at(x).at(y).push_back(tank);
    tank->currentCell = GridVector.at(x).at(y);
}

vec2 Grid::Grid::GetGridPoint(vec2 position)
{
    return vec2(floor(position.x / cellSize) + 100, floor(position.y / cellSize) + 100);
}

std::vector<std::vector<Tank*>> Grid::Grid::CollisionCheck(Rocket& rocket)
{
    vec2 gridPnt = GetGridPoint(rocket.position);
    std::vector<std::vector<Tank*>> tanks;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i + gridPnt.x >= 0 && j + gridPnt.y >= 0)
            {
                std::vector<Tank*> tankVec = GridVector.at(i + gridPnt.x).at(j + gridPnt.y);
                tanks.emplace_back(tankVec);
            }
        }
    }
    return tanks;
}

Tank* Grid::Grid::FindNearestNeighbor(Tank* tank)
{
    std::vector<Tank*>& currentCell = tank->currentCell;
    std::vector<std::vector<Tank*>> todoCells;
    std::vector<std::vector<Tank*>> collideCells;
    int length = 0;
    Tank* targetTank;
    vec2 currentCellPosition = tank->position;
    vec2 direction = down;
    float largestDistance = INFINITE;
    int depth = 0;
    //do
    //{
    //    depth++;
    //    for (int i = 0; i < 4; i++)
    //    {
    //        direction = (i == 0 ? right : (i == 1 ? down : (i == 2 ? left : right))) * vec2{(float) depth * 2, (float) depth * 2};
    //        for (int j = 0; j < (direction.x != 0 ? direction.x : direction.y); j++)
    //        {
    //            std::vector<Tank*> cell = GridVector.at((direction.x != 0 ? direction.x : 0) + currentCellPosition.x).at((direction.y != 0 ? direction.y : 0) + currentCellPosition.y);
    //            if (!cell.empty())
    //            {
    //                collideCells.push_back(cell);
    //            }
    //        }
    //    }
    //} while (collideCells.empty());
    for (std::vector<Tank*> cell : collideCells)
    {
        for (Tank* nearTank : cell)
        {
            vec2 dir = tank->position - nearTank->position;
            float dirSquaredLen = dir.sqrLength();
            if (dirSquaredLen > largestDistance)
            {
                targetTank = nearTank;
                largestDistance = dirSquaredLen;
            }
        }
    }
    return targetTank;
}