#pragma once

#include "exeptions.h"
#include <gtest/gtest_prod.h>

#include <list>
#include <map>
#include <memory>


class command
{
public:
  command () = default;
  virtual ~command () = default;

  virtual void execute () = 0;
};

class exception_handler
{
using exception_handler_map_type = std::unordered_map<info_ref, std::function<void (exception_base *, std::unique_ptr<command>)>, hasher, equal>;
public:
  exception_handler () = default;
  ~exception_handler () = default;

  void run (exception_base *, std::unique_ptr<command>);
  void register_handler (const std::type_info &info, const std::function<void (exception_base *, std::unique_ptr<command>)> &handler);

private:
  exception_handler_map_type map;
};

class command_pool : public command
{
public:
  command_pool (exception_handler &exception_handler_ref) : exception_handler_ref (exception_handler_ref) {}
  virtual ~command_pool () = default;

  virtual void execute () override;
  void pop (std::unique_ptr<command> command);
  bool empty () const { return pool.empty (); }

private:
  std::list<std::unique_ptr<command>> pool;
  exception_handler &exception_handler_ref;

  FRIEND_TEST(exeption_handling, exeption_handling_test);
};

class print_to_log_command : public command
{
public:
  print_to_log_command (const std::string &message) : message (message) {}
  virtual ~print_to_log_command () = default;

  virtual void execute () override;

private:
  std::string message;
};

class repeater : public command
{
public:
  repeater (std::unique_ptr<command> command_to_repeat) : command_to_repeat (std::move (command_to_repeat)) {}
  virtual ~repeater () = default;

  virtual void execute () override;

private:
  std::unique_ptr<command> command_to_repeat;
};

class unknown_command : public command
{
public:
  unknown_command () = default;
  virtual ~unknown_command () = default;

  virtual void execute () override {}
};
