#pragma once

#include <gtest/gtest.h>

#include "commands.h"

using namespace testing;

class mock_command_with_error : public unknown_command
{
public:
  mock_command_with_error () = default;
  virtual ~mock_command_with_error () = default;

  virtual void execute () override { throw exception_base ("Mock exeption"); }
};

TEST(exeption_handling, exeption_handling_test)
{
  exception_handler handler;
  command_pool pool (handler);
  handler.register_handler (typeid (repeater), [&pool] (exception_base *exception_ptr, std::unique_ptr<command>)
  {
    std::unique_ptr<command> print_to_log (new print_to_log_command (exception_ptr->get_message ()));
    pool.pop (std::move (print_to_log));
  });
  handler.register_handler (typeid (unknown_command), [&pool] (exception_base *, std::unique_ptr<command> command_ptr)
  {
    std::unique_ptr<command> repeater_command (new repeater (std::move (command_ptr)));
    pool.pop (std::move (repeater_command));
  });

  pool.pop (std::make_unique<mock_command_with_error> ());
  pool.execute ();
  EXPECT_TRUE (pool.pool.size () == 1);
  auto &repeater_ref = *pool.pool.front ().get ();
  EXPECT_TRUE (typeid (repeater_ref) == typeid (repeater));
  pool.execute ();
  EXPECT_TRUE (pool.pool.size () == 1);
  auto &print_ref = *pool.pool.front ().get ();
  EXPECT_TRUE (typeid (print_ref) == typeid (print_to_log_command));
  pool.execute ();
  EXPECT_TRUE (pool.pool.size () == 0);
}
