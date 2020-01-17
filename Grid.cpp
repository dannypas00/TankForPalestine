#include "precomp.h"

std::vector<std::vector<vector<Tank*>>> GridVector;
UINT16 cellSize;
Grid::Grid::Grid(int width, int height, UINT16 cSize)
{
    cellSize = cSize;
    std::vector<std::vector<vector<Tank*>>> xVector;
    for (int x = (width / cSize) - 1; x >= 0; x--)
    {
        std::vector<std::vector<Tank*>> yVector;
        for (int y = (height / cSize) - 1; y >= 0; y--)
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
    return vec2(floor(position.x / cellSize), floor(position.y / cellSize));
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
    for (std::vector<Tank*> cell : collideCells) {
        for (Tank* nearTank : cell) {
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