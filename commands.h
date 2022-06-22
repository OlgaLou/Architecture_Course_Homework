#pragma once

#include <memory>
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
  macro_command (std::vector<std::unique_ptr<command>> &&commands) : commands (std::move (commands)) {}
  virtual ~macro_command () = default;

  virtual void execute () override
  {
    try
    {
      for (const auto &any_command : commands)
        any_command->execute ();
    }
    catch (const exception_base &value)
    {
      throw value;
    }
  }

  size_t count () { return commands.size (); }

private:
  std::vector<std::unique_ptr<command>> commands;
};
