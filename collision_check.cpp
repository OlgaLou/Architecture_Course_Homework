#include "collision_check.h"

#include <memory>

std::map<int, locality_system *> locality_system::systems_map;
int locality_system::last_id = -1;

std::unordered_map<int, std::map<movable_iface *, std::unique_ptr<macro_command>>> collision_checkers::checkers_map;

locality_system::locality_system ()
{
  last_id++;
  locality_id = last_id;
  systems_map[locality_id] = this;
}

locality_system::~locality_system ()
{
  systems_map.erase (locality_id);
}

int locality_system::find_object (movable_iface *movable) const
{
  for (auto &zone_objects : zone_to_objects_map)
  {
    if (zone_objects.second.count (movable))
      return zone_objects.first;
  }
  return -1;
}

void locality_system::add_object (int zone_num, movable_iface *movable)
{
  zone_to_objects_map[zone_num].insert (movable);
}

void locality_system::remove_object (int zone_num, movable_iface *movable)
{
  if (!zone_to_objects_map.count (zone_num))
    throw exception_base ("Unknown zone");
  auto it = zone_to_objects_map[zone_num].find (movable);
  if (it == zone_to_objects_map[zone_num].end ())
    throw exception_base ("Object not found in zone");
  zone_to_objects_map[zone_num].erase (it);
}

const std::set<movable_iface *> &locality_system::get_children (int zone_num) const
{
  if (!zone_to_objects_map.count (zone_num))
    throw exception_base ("Unknown zone");

  return zone_to_objects_map.at (zone_num);
}

void collision_checkers::insert_checkers (movable_iface *movable, int local_id, std::unique_ptr<macro_command> &&collision_checkers)
{
  checkers_map[local_id][movable] = std::move (collision_checkers);
}

macro_command *collision_checkers::get_checkers (movable_iface *movable, int local_id)
 {
   if (!checkers_map.count (local_id))
     return nullptr;

   if (!checkers_map[local_id].count (movable))
     return nullptr;

   return checkers_map[local_id][movable].get ();
 }

void collision_check_one_to_one::execute ()
{
  if (is_collided ())
    throw collision_check_exception ();
}

void collision_check_one_to_many::execute ()
{
  point position;
  try
  {
    position = movable->get_position ();
  }
  catch (const get_exception &value)
  {
    throw value;
  }

  int current_zone_num = -1, previuos_zone_num = -1;
  try
  {
    current_zone_num = system.get_zone_num (position);
    previuos_zone_num = system.find_object (movable);
  }
  catch (const exception_base &value)
  {
    throw command_exception ("Zone updating error: " + value.get_message ());
  }

  if (current_zone_num == previuos_zone_num)
    return;

  try
  {
    system.remove_object (previuos_zone_num, movable);
    system.add_object (current_zone_num, movable);
  }
  catch (const exception_base &)
  {
    throw command_exception ("Error during update locality maps");
  }

  std::vector<std::unique_ptr<command>> collision_checkers;
  for (movable_iface *child : system.get_children (current_zone_num))
  {
    if (child == movable)
      continue;

    collision_checkers.push_back (std::unique_ptr<collision_check_one_to_one> (new collision_check_one_to_one (movable, child, system)));
  }
  collision_checkers::insert_checkers (movable, system.get_id (), std::unique_ptr<macro_command> (new macro_command (std::move (collision_checkers))));
}
