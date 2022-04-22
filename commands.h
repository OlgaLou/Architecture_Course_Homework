#pragma once

#include <vector>

#include "exceptions.h"

class command
{
public:
  command () = default;
  virtual ~command () = default;

  virtual void execute () = 0;
};

class macro_command : public command
{
public:
  macro_command (const std::vector<command *> &commands) : commands (commands) {}
  virtual ~macro_command () = default;

  virtual void execute () override
  {
    try
    {
      for (command *any_command : commands)
        any_command->execute ();
    }
    catch (const command_exception &value)
    {
      throw command_exception ("Macro Command Error - " + value.get_message ());
    }
  }

private:
  std::vector<command *> commands;
};
