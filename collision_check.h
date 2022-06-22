#pragma once

#include "commands.h"
#include "exceptions.h"
#include "objects.h"
#include <map>
#include <set>
#include <unordered_map>

class locality_system
{
public:
  locality_system ();
  ~locality_system ();

  virtual void init (rectangle /*field_geometry*/, int /*horizontal_locality_count*/, int /*vertical_locality_count*/, point /*center_shift*/) { }
  int get_id () const { return locality_id; }
  virtual int get_zone_num (point p) const = 0;
  virtual int find_object (movable_iface *movable) const;
  virtual void add_object (int zone_num, movable_iface *movable);
  virtual void remove_object (int zone_num, movable_iface *movable);
  virtual const std::set<movable_iface *> &get_children (int zone_num) const;

  static std::map<int, locality_system *>& get_systems_map () { return systems_map; }

private:
  int locality_id = -1;
  std::map<int, rectangle> zone_to_geometry_map;
  std::map<int, std::set<movable_iface *>> zone_to_objects_map;

  static int last_id;
  static std::map<int, locality_system *> systems_map;
};

class collision_checkers
{
public:
  static void insert_checkers (movable_iface *movable, int local_id, std::unique_ptr<macro_command> &&collision_checkers);
  static macro_command *get_checkers (movable_iface *movable, int local_id);
private:
  static std::unordered_map<int, std::map<movable_iface *, std::unique_ptr<macro_command>>> checkers_map;
};

class collision_check_one_to_one : public command
{
public:
  collision_check_one_to_one (movable_iface *current_movable, movable_iface *movable, locality_system &system)
    : current_movable (current_movable), movable (movable), system (system)
  {}

  virtual void execute () override;
  virtual bool is_collided () const { return true; }

private:
  movable_iface *current_movable = nullptr;
  movable_iface *movable = nullptr;
  locality_system &system;
};

class collision_check_one_to_many : public command
{
public:
  collision_check_one_to_many (movable_iface *movable, locality_system &system)
    : movable (movable), system (system)
  {}

  virtual void execute () override;

private:
  movable_iface *movable = nullptr;
  locality_system &system;
};
