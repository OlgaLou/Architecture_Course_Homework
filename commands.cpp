#include "commands.h"

#include <iostream>

void exception_handler::register_handler (const std::type_info &info, const std::function<void (exception_base *, std::unique_ptr<command>)> &handler)
{
  map[info] = handler;
}

void exception_handler::run (exception_base *exception_ptr, std::unique_ptr<command> command_ptr)
{
  auto& ref = *command_ptr.get ();
  if (map.count (typeid (ref)))
  {
    map.at (typeid (ref)) (exception_ptr, std::move (command_ptr));
    return;
  }
  if (map.count (typeid (unknown_command)))
  {
    map.at (typeid (unknown_command)) (exception_ptr, std::move (command_ptr));
    return;
  }
  throw exception_handler_error ();
}

void command_pool::execute ()
{
  if (pool.empty ())
    return;

  std::unique_ptr<command> local_command = std::move (pool.front ());
  try
  {
    local_command->execute ();
  }
  catch (exception_base &err)
  {
    exception_handler_ref.run (&err, std::move (local_command));
  }
  pool.pop_front ();
}

void command_pool::pop (std::unique_ptr<command_pool::command> command)
{
  pool.push_back (std::move (command));
}

void print_to_log_command::execute ()
{
  std::cout << message << std::endl;
}

void repeater::execute ()
{
  if (!command_to_repeat || !command_to_repeat.get ())
    throw repeater_exeption ();
  command_to_repeat->execute ();
}
