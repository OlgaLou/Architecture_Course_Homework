#pragma once

#include <functional>
#include <typeinfo>

class exception_base
{
  public:
    exception_base (const char *value) : value (value) {}
    const char *get_message () const { return value; }
  private:
    const char *value = nullptr;
};

class exception_handler_error : public exception_base
{
public:
  exception_handler_error () : exception_base ("No Exeption Handler") {}
};

class repeater_exeption : public exception_base
{
public:
  repeater_exeption () : exception_base ("Error During Repetition") {}
};

using info_ref = std::reference_wrapper<const std::type_info>;
struct hasher
{
    std::size_t operator () (info_ref code) const
    {
        return code.get ().hash_code ();
    }
};

struct equal {
  bool operator() (info_ref lhs, info_ref rhs) const
  {
    return lhs.get() == rhs.get();
  }
};
