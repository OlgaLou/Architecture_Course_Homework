#pragma once

#include "commands.h"
#include "motion.h"

class fuel_processing_iface
{
public:
  fuel_processing_iface () {}
  virtual ~fuel_processing_iface () = default;

  virtual int get_fuel_reserve () const = 0;
  virtual int get_burn_fuel_velocity () const = 0;
  virtual void set_fuel_reserve (int) = 0;
};

class check_fuel : public command
{
public:
  check_fuel (const fuel_processing_iface *fuel);
  ~check_fuel () = default;

  virtual void execute () override;
private:
  const fuel_processing_iface *fuel = nullptr;
};

class burn_fuel : public command
{
public:
  burn_fuel (fuel_processing_iface *fuel);
  ~burn_fuel () = default;

  virtual void execute () override;
private:
  fuel_processing_iface *fuel = nullptr;
};

class motion_with_burn_fuel : public macro_command
{
public:
  motion_with_burn_fuel (check_fuel *check_fuel_command, move *move_command, burn_fuel *burn_fuel_command) : macro_command ({check_fuel_command, move_command, burn_fuel_command}) {}
  ~motion_with_burn_fuel () = default;
};

