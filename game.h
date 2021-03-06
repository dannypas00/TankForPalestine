#pragma once

namespace Tmpl8
{
//forward declarations
class Tank;
class Rocket;
class Smoke;
class Particle_beam;

class Game
{
  public:
    void SetTarget(Surface* surface) { screen = surface; }
    void Init();
    void Shutdown();
    void Update(float deltaTime);
    void Draw();
    void Tick(float deltaTime);
    //void insertion_sort_tanks_health(const std::vector<Tank>& original, std::vector<const Tank*>& sorted_tanks, UINT16 begin, UINT16 end);
    void MeasurePerformance();
    void ParallelTankSpawn(size_t, float, float, float, float, float, uint, uint);

    Tank& FindClosestEnemy(Tank& current_tank);

    void MassCollisionCheck(std::vector<Tank*>& sortedTanks, std::vector<Tank*>& activeTanks, int beginT, int endT);

    void CollisionCheck(std::vector<Tank*> unsorted);

    Tank& FindClosestEnemyParallel(Tank& current_tank);

    void MouseUp(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void MouseDown(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void MouseMove(int x, int y)
    { /* implement if you want to detect mouse movement */
    }

    void KeyUp(int key)
    { /* implement if you want to handle keys */
    }

    void KeyDown(int key)
    { /* implement if you want to handle keys */
    }

  private:
    Surface* screen;

    vector<Tank> tanks;
    vector<Rocket> rockets;
    vector<Smoke> smokes;
    vector<Explosion> explosions;
    vector<Particle_beam> particle_beams;
    vector<Tank*> sorted;
    vector<Tank*> colsorted;
    vector<Tank*> bluetanks;
    vector<Tank*> redtanks;

    future<void> f1;

    Font* frame_count_font;
    long long frame_count = 0;

    bool lock_update = false;
    bool rocketTick = false;
    bool beamTick = false;
};

}; // namespace Tmpl8