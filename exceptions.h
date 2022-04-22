#pragma once

#include <string>

class exception_base
{
  public:
    exception_base (const std::string &value) : value (value) {}
    std::string get_message () const { return value; }
  private:
    std::string value;
};

class command_exception : public exception_base
{
public:
  command_exception (const std::string &value = "Unknown") : exception_base ("Command Error: " + value) {}
};

class move_exception : public command_exception
{
public:
  move_exception () : command_exception ("Motion Error") {}
};

class get_exception : public exception_base
{
public:
  get_exception () : exception_base ("Object Property Getter Error") {}
};

class set_exception : public exception_base
{
public:
  set_exception () : exception_base ("Object Property Setter Error") {}
};

class rotate_exception : public command_exception
{
public:
  rotate_exception () : command_exception ("Rotation Error") {}
};

class change_velocity_exception : public command_exception
{
public:
  change_velocity_exception () : command_exception ("Change Velocity Error") {}
};
