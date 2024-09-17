#include "fuel_commands.h"

#include "exceptions.h"


check_fuel::check_fuel (const fuel_processing_iface *fuel)
  : fuel (fuel)
{

}

void check_fuel::execute ()
{
  int fuel_reserve = 0, burn_fuel_velocity = 0;
  try
  {
    fuel_reserve = fuel->get_fuel_reserve ();
    burn_fuel_velocity = fuel->get_burn_fuel_velocity ();
  } catch (const get_exception &value)
  {
    throw command_exception (value.get_message ());
  }
  if (fuel_reserve < burn_fuel_velocity)
    throw command_exception ("Not Enough Fuel");
}

burn_fuel::burn_fuel (fuel_processing_iface *fuel)
  : fuel (fuel)
{

}

void burn_fuel::execute ()
{
  try
  {
    int fuel_reserve = fuel->get_fuel_reserve ();
    int burn_fuel_velocity = fuel->get_burn_fuel_velocity ();
    fuel->set_fuel_reserve (fuel_reserve - burn_fuel_velocity);
  }
  catch (const get_exception &value)
  {
    throw command_exception (value.get_message ());
  }
  catch (const set_exception &value)
  {
    throw command_exception (value.get_message ());
  }
}
