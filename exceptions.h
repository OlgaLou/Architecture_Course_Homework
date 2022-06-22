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

class collision_check_exception : public exception_base
{
public:
  collision_check_exception () : exception_base ("Objects Collided!") {}
};

