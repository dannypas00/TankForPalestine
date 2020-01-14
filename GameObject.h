#pragma once
#include "precomp.h"

namespace Tmpl8
{
class GameObject
{
  public:
    virtual void Tick();
    virtual void Draw(Surface* screen);

    vec2 Get_Position() const { return position; };

    vec2 position;
    vec2 speed;

    float collision_radius;

    allignments allignment;

    bool active;
};
} // namespace Tmpl8