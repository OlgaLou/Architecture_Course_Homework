#pragma once

#include <math.h>

#include "commands.h"
#include "exceptions.h"
#include "rotation.h"

#define PI 3.14159265

struct tuple_2d
{
  tuple_2d (int x = 0, int y = 0) : x (x), y (y) {}
  int x = 0;
  int y = 0;
};

class movable_and_rotating_iface
{
public:
  movable_and_rotating_iface () {}
  virtual ~movable_and_rotating_iface () = default;

  virtual int get_direction () const = 0;
  virtual int get_direction_count () const = 0;
  virtual tuple_2d get_velocity () const = 0;
  virtual void set_velocity (const tuple_2d &velocity) = 0;
};

class change_velocity : public command
{
public:
  change_velocity (movable_and_rotating_iface *movable_and_rotating) : movable_and_rotating (movable_and_rotating) {}
  ~change_velocity () = default;

  virtual void execute () override
  {
    try
    {
      int direction = movable_and_rotating->get_direction ();
      int direction_count = movable_and_rotating->get_direction_count ();
      tuple_2d velocity = movable_and_rotating->get_velocity ();
      double degree_to_radian = PI / 180;
      double angle = degree_to_radian * direction * 360 / direction_count;
      movable_and_rotating->set_velocity (tuple_2d (velocity.x * cos (angle), velocity.y * sin (angle)));
    }
    catch (const get_exception &)
    {
      throw move_exception ();
    }
    catch (const set_exception &)
    {
      throw move_exception ();
    }
  }

private:
  movable_and_rotating_iface *movable_and_rotating = nullptr;
};

class rotate_and_change_velocity : public macro_command
{
public:
  rotate_and_change_velocity (rotate *rotate_command, change_velocity *change_velocity_command) : macro_command ({rotate_command, change_velocity_command}) {}
  ~rotate_and_change_velocity () = default;
};
