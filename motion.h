#pragma once

#include <utility>

#include "commands.h"
#include "exceptions.h"


class movable_iface
{
public:
  movable_iface () {}
  virtual ~movable_iface () = default;

  virtual std::pair<int, int> get_position () const = 0;
  virtual std::pair<int, int> get_velocity () const = 0;
  virtual void set_position (const std::pair<int, int> &position) = 0;
};

class move : public command
{
public:
  move (movable_iface *movable) : movable (movable) {}
  ~move () = default;

  virtual void execute () override
  {
    try
    {
      auto position = movable->get_position ();
      auto velocity = movable->get_velocity ();
      movable->set_position (std::make_pair (position.first + velocity.first, position.second + velocity.second));
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
  movable_iface *movable = nullptr;
};
