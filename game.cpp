#include "precomp.h" // include (only) this in every .cpp file
#include <array>
#include <iostream>
#include <ppl.h>
#include <sstream>
#include <windows.h>

#define NUM_TANKS_BLUE 1279
#define NUM_TANKS_RED 1279

#define TANK_MAX_HEALTH 1000
#define ROCKET_HIT_VALUE 60
#define PARTICLE_BEAM_HIT_VALUE 50

#define TANK_MAX_SPEED 1.5

#define HEALTH_BARS_OFFSET_X 0
#define HEALTH_BAR_HEIGHT 70
#define HEALTH_BAR_WIDTH 1
#define HEALTH_BAR_SPACING 0

#define MAX_FRAMES 2000

//Global performance timer
//REF_PERFORMANCE NICK: 57137.9
//REF_PERFORMANCE DANNY: 60245.9
#define REF_PERFORMANCE 58849.8 //UPDATE THIS WITH YOUR REFERENCE PERFORMANCE (see console after 2k frames)
static timer perf_timer;
static float duration;

//Load sprite files and initialize sprites
static Surface* background_img = new Surface("assets/Background_Grass.png");
static Surface* tank_red_img = new Surface("assets/Tank_Proj2.png");
static Surface* tank_blue_img = new Surface("assets/Tank_Blue_Proj2.png");
static Surface* rocket_red_img = new Surface("assets/Rocket_Proj2.png");
static Surface* rocket_blue_img = new Surface("assets/Rocket_Blue_Proj2.png");
static Surface* particle_beam_img = new Surface("assets/Particle_Beam.png");
static Surface* smoke_img = new Surface("assets/Smoke.png");
static Surface* explosion_img = new Surface("assets/Explosion.png");

static Sprite background(background_img, 1);
static Sprite tank_red(tank_red_img, 12);
static Sprite tank_blue(tank_blue_img, 12);
static Sprite rocket_red(rocket_red_img, 12);
static Sprite rocket_blue(rocket_blue_img, 12);
static Sprite smoke(smoke_img, 4);
static Sprite explosion(explosion_img, 9);
static Sprite particle_beam_sprite(particle_beam_img, 3);

const static vec2 tank_size(14, 18);
const static vec2 rocket_size(25, 24);

const static float tank_radius = 12.f;
const static float rocket_radius = 10.f;

Grid grid = Grid(9999, 9999);
std::mutex hitLock;

namespace
{
const unsigned int threadCount = thread::hardware_concurrency();

ThreadPool pool(threadCount);
} // namespace

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    frame_count_font = new Font("assets/digital_small.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ:?!=-0123456789.");

    tanks.reserve(NUM_TANKS_BLUE + NUM_TANKS_RED);

    uint rows = (uint)sqrt(NUM_TANKS_BLUE + NUM_TANKS_RED);
    uint max_rows = 12;

    float start_blue_x = tank_size.x + 10.0f;
    float start_blue_y = tank_size.y + 80.0f;

    float start_red_x = 980.0f;
    float start_red_y = 100.0f;

    float spacing = 15.0f;

    //Spawn blue tanks
    for (int i = 0; i < NUM_TANKS_BLUE; i++)
    {
        Tank tank = Tank(start_blue_x + ((i % max_rows) * spacing), start_blue_y + ((i / max_rows) * spacing), BLUE, &tank_blue, &smoke, 1200, 600, tank_radius, TANK_MAX_HEALTH, TANK_MAX_SPEED);
        tank.oldGridPoint = Grid::GetGridPoint(tank.position);
        tanks.push_back(tank);
        sorted.push_back(&tanks.back());
        grid.FillGrid(&tank);
    }
    //Spawn red tanks
    for (int i = 0; i < NUM_TANKS_RED; i++)
    {
        Tank tank = Tank(start_red_x + ((i % max_rows) * spacing), start_red_y + ((i / max_rows) * spacing), RED, &tank_red, &smoke, 80, 80, tank_radius, TANK_MAX_HEALTH, TANK_MAX_SPEED);
        tank.oldGridPoint = Grid::GetGridPoint(tank.position);
        tanks.push_back(tank);
        sorted.push_back(&tanks.back());
        grid.FillGrid(&tank);
    }

    particle_beams.push_back(Particle_beam(vec2(SCRWIDTH / 2, SCRHEIGHT / 2), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));
    particle_beams.push_back(Particle_beam(vec2(80, 80), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));
    particle_beams.push_back(Particle_beam(vec2(1200, 600), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));
}

// -----------------------------------------------------------
// Close down application
// -----------------------------------------------------------
void Game::Shutdown()
{
}

// -----------------------------------------------------------
// Iterates through all tanks and returns the closest enemy tank for the given tank
// -----------------------------------------------------------
Tank& Game::FindClosestEnemy(Tank& current_tank)
{
    float closest_distance = numeric_limits<float>::infinity();
    int closest_index = 0;

    for (int i = 0; i < tanks.size(); i++)
    {
        if (tanks.at(i).allignment != current_tank.allignment && tanks.at(i).active)
        {
            float sqrDist = fabsf((tanks.at(i).Get_Position() - current_tank.Get_Position()).sqrLength());
            if (sqrDist < closest_distance)
            {
                closest_distance = sqrDist;
                closest_index = i;
            }
        }
    }

    return tanks.at(closest_index);
}


// -----------------------------------------------------------
// Collision detection by implementing a custom hyperplane seperation algorithm
// -----------------------------------------------------------
void Game::MassCollisionCheck(std::vector<Tank*>& sortedTanks, std::vector<Tank*>& activeTanks, int beginT, int endT)
{
    std::vector<Tank*> stepTanks(&activeTanks[beginT], &activeTanks[endT]);
    for (int i = 0; i < stepTanks.size(); i++)
    {
        int l = distance(&activeTanks.at(0), std::find(&activeTanks.at(0), &activeTanks.at(activeTanks.size() - 1), stepTanks.at(i)));
        Tank* currTank = activeTanks.at(l);
        int x = currTank->position.x;
        int y = currTank->position.y;
        int j = l + 1;
        int k = l - 1;

        while (j < sortedTanks.size() && sortedTanks.at(j)->position.x <= x + (2 * tank_radius))
        {
            if (sortedTanks.at(j) != currTank)
            {
                vec2 dir = currTank->position - sortedTanks.at(j)->position;
                float dirSquaredLen = dir.sqrLength();

                float colSquaredLen = (tank_radius * tank_radius * 2);

                if (dirSquaredLen < colSquaredLen)
                {
                    currTank->Push(dir.normalized(), 1.f);
                }
            }
            j++;
        }
        while (k > 0 && sortedTanks.at(k)->position.x <= x + (2 * tank_radius))
        {
            if (sortedTanks.at(k) != currTank)
            {
                vec2 dir = currTank->position - sortedTanks.at(k)->position;
                float dirSquaredLen = dir.sqrLength();

                float colSquaredLen = (tank_radius * tank_radius * 2);

                if (dirSquaredLen < colSquaredLen)
                {
                    currTank->Push(dir.normalized(), 1.f);
                }
            }
            k--;
        }
    }
}

// -----------------------------------------------------------
// Update the game state:
// -----------------------------------------------------------
void Game::Update(float deltaTime)
{
    std::vector<future<void>> fut;
    std::vector<Tank*> unsorted;
    std::vector<Tank*> active;
    beamTick = false;
    rocketTick = false;

    //Update tanks
    for (Tank& tank : tanks)
    {
        if (tank.health <= 0 && tank.active)
        {
            smokes.push_back(Smoke(smoke, tank.position - vec2(0, 48)));
            tank.Deactivate();
        }
        unsorted.push_back(&tank);
        if (tank.active)
        {
            active.push_back(&tank);
            tank.Tick();

            // -----------------------------------------------------------
            // Rocket intersection by filling a grid with tanks
            // -----------------------------------------------------------
            vec2& gridPoint = Grid::GetGridPoint(tank.position);
            if (gridPoint.x != tank.oldGridPoint.x || gridPoint.y != tank.oldGridPoint.y)
            {
                std::vector<Tank*>& oldGridTanks = grid.GridVector.at(tank.oldGridPoint.x).at(tank.oldGridPoint.y);
                std::remove(oldGridTanks.begin(), oldGridTanks.end(), &tank);
                grid.FillGrid(&tank);
                tank.oldGridPoint = gridPoint;
            }

            for (Particle_beam& particle_beam : particle_beams)
            {
                if (particle_beam.rectangle.intersectsCircle(tank.position, tank_radius))
                {
                    if (tank.hit(particle_beam.damage))
                    {
                        smokes.push_back(Smoke(smoke, tank.position - vec2(0, 48)));
                    }
                }
            }

            //Shoot at closest target if reloaded
            if (tank.Rocket_Reloaded())
            {
                Tank& target = FindClosestEnemy(tank);

                rockets.push_back(Rocket(tank.position, (target.Get_Position() - tank.position).normalized() * 3, rocket_radius, tank.allignment, ((tank.allignment == RED) ? &rocket_red : &rocket_blue)));

                tank.Reload_Rocket();
            }
        }
        else
        {
            vec2& gridPoint = Grid::GetGridPoint(tank.position);
            std::vector<Tank*>& oldGridTanks = grid.GridVector.at(tank.oldGridPoint.x).at(tank.oldGridPoint.y);
            std::remove(oldGridTanks.begin(), oldGridTanks.end(), &tank);
        }
    }

    int rocketStep = ceil(rockets.size() / threadCount);
    for (int i = 0; i < threadCount; i++)
    {
        fut.emplace_back(pool.enqueue([&, i] {
            int endIndex = (rocketStep * i) + rocketStep - 1;
            if ((rocketStep * i) + rocketStep - 1 > rockets.size() - 1)
            {
                endIndex = rockets.size() - 1;
            }
            for (int j = i * rocketStep; j <= endIndex; j++)
            {
                Rocket& rocket = rockets.at(j);
                rocket.Tick();
                for (std::vector<Tank*>*& collisions : grid.CollisionCheck(rocket))
                {
                    for (Tank*& tank : *collisions)
                    {
                        if (tank->allignment != rocket.allignment && tank->active && rocket.active && tank->health > 0)
                        {
                            float distance_sqr = (tank->position - rocket.position).sqrLength();

                            if (distance_sqr <= ((rocket.collision_radius * rocket.collision_radius) + (tank_radius * tank_radius)))
                            {
                                std::lock_guard<std::mutex> lock(hitLock);
                                explosions.push_back(Explosion(&explosion, tank->position));
                                tank->hit(ROCKET_HIT_VALUE);
                                rocket.active = false;
                                break;
                            }
                        }
                    }
                }
                if (rocket.position.x > 9999 || rocket.position.x < -100 || rocket.position.y > 9999 || rocket.position.y < -100)
                {
                    rocket.active = false;
                    break;
                }
            }
        }));
    }
    for (future<void>& f : fut)
    {
        f.wait();
    }
    fut.clear();

    int step = ceil(active.size() / threadCount);
    Mergesort::mergesort::poolXSort(unsorted, 0, unsorted.size() - 1, 1);
    Mergesort::mergesort::poolXSort(active, 0, active.size() - 1, 1);
    for (int i = 0; i < threadCount; i++) //Don't pass i by reference but by value
    {
        fut.emplace_back(pool.enqueue([&, i] {
            int endIndex = (step * i) + step - 1;
            if ((step * i) + step - 1 > active.size() - 1)
            {
                endIndex = active.size() - 1;
            }
            MassCollisionCheck(unsorted, active, step * i, endIndex);
        }));
    }
    for (future<void>& f : fut)
    {
        f.wait();
    }

    for (Particle_beam& particle_beam : particle_beams)
    {
        particle_beam.tick(tanks);
    }

    //Update smoke plumes
    for (Smoke& smoke : smokes)
    {
        smoke.Tick();
    }

    //Remove exploded rockets with remove erase idiom
    rockets.erase(std::remove_if(rockets.begin(), rockets.end(), [](const Rocket& rocket) { return !rocket.active; }), rockets.end());

    //Update explosion sprites and remove when done with remove erase idiom
    for (Explosion& explosion : explosions)
    {
        explosion.Tick();
    }

    explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [](const Explosion& explosion) { return explosion.done(); }), explosions.end());
}

void Game::Draw()
{
    // clear the graphics window
    screen->Clear(0);

    //Draw background
    background.Draw(screen, 0, 0);

    //Draw sprites
    for (int i = 0; i < NUM_TANKS_BLUE + NUM_TANKS_RED; i++)
    {
        tanks.at(i).Draw(screen);

        vec2 tPos = tanks.at(i).Get_Position();
        // tread marks
        if ((tPos.x >= 0) && (tPos.x < SCRWIDTH) && (tPos.y >= 0) && (tPos.y < SCRHEIGHT))
            background.GetBuffer()[(int)tPos.x + (int)tPos.y * SCRWIDTH] = SubBlend(background.GetBuffer()[(int)tPos.x + (int)tPos.y * SCRWIDTH], 0x808080);
    }

    for (Rocket& rocket : rockets)
    {
        rocket.Draw(screen);
    }

    for (Smoke& smoke : smokes)
    {
        smoke.Draw(screen);
    }

    for (Particle_beam& particle_beam : particle_beams)
    {
        particle_beam.Draw(screen);
    }

    for (Explosion& explosion : explosions)
    {
        explosion.Draw(screen);
    }

    //Draw sorted health bars
    for (int t = 0; t < 2; t++)
    {
        const UINT16 NUM_TANKS = ((t < 1) ? NUM_TANKS_BLUE : NUM_TANKS_RED);

        const UINT16 begin = ((t < 1) ? 0 : NUM_TANKS_BLUE);
        Mergesort::mergesort::poolHealthSort(sorted, begin, begin + NUM_TANKS - 1, 1);

        for (int i = 0; i < NUM_TANKS; i++)
        {
            int health_bar_start_x = i * (HEALTH_BAR_WIDTH + HEALTH_BAR_SPACING) + HEALTH_BARS_OFFSET_X;
            int health_bar_start_y = (t < 1) ? 0 : (SCRHEIGHT - HEALTH_BAR_HEIGHT) - 1;
            int health_bar_end_x = health_bar_start_x + HEALTH_BAR_WIDTH;
            int health_bar_end_y = (t < 1) ? HEALTH_BAR_HEIGHT : SCRHEIGHT - 1;

            screen->Bar(health_bar_start_x, health_bar_start_y, health_bar_end_x, health_bar_end_y, REDMASK);
            screen->Bar(health_bar_start_x, health_bar_start_y + (int)((double)HEALTH_BAR_HEIGHT * (1 - ((double)sorted.at(i)->health / (double)TANK_MAX_HEALTH))), health_bar_end_x, health_bar_end_y, GREENMASK);
        }
    }
}

// -----------------------------------------------------------
// When we reach MAX_FRAMES print the duration and speedup multiplier
// Updating REF_PERFORMANCE at the top of this file with the value
// on your machine gives you an idea of the speedup your optimizations give
// -----------------------------------------------------------
void Tmpl8::Game::MeasurePerformance()
{
    char buffer[128];
    if (frame_count >= MAX_FRAMES)
    {
        if (!lock_update)
        {
            duration = perf_timer.elapsed();
            cout << "Duration was: " << duration << " (Replace REF_PERFORMANCE with this value)" << endl;
            lock_update = true;
        }

        frame_count--;
    }

    if (lock_update)
    {
        screen->Bar(420, 170, 870, 430, 0x030000);
        int ms = (int)duration % 1000, sec = ((int)duration / 1000) % 60, min = ((int)duration / 60000);
        sprintf(buffer, "%02i:%02i:%03i", min, sec, ms);
        frame_count_font->Centre(screen, buffer, 200);
        sprintf(buffer, "SPEEDUP: %4.1f", REF_PERFORMANCE / duration);
        frame_count_font->Centre(screen, buffer, 340);
    }
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
    if (!lock_update)
    {
        Update(deltaTime);
    }
    Draw();

    MeasurePerformance();

    // print something in the graphics window
    //screen->Print("hello world", 2, 2, 0xffffff);

    // print something to the text window
    //cout << "This goes to the console window." << std::endl;

    //Print frame count
    frame_count++;
    string frame_count_string = "FRAME: " + std::to_string(frame_count);
    frame_count_font->Print(screen, frame_count_string.c_str(), 350, 580);
}
