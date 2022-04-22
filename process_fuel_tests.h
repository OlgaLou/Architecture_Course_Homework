#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "exceptions.h"
#include "fuel_commands.h"

using namespace testing;

class mock_fuel : public fuel_processing_iface
{
public:
  MOCK_CONST_METHOD0 (get_fuel_reserve, int ());
  MOCK_CONST_METHOD0 (get_burn_fuel_velocity, int ());
  MOCK_METHOD1 (set_fuel_reserve, void (int));
};

TEST(check_fuel_tests, fuel_sufficiency_test)
{
  mock_fuel mock_obj;
  EXPECT_CALL(mock_obj, get_fuel_reserve ())
          .WillOnce (Return (10));
  EXPECT_CALL(mock_obj, get_burn_fuel_velocity ())
          .WillOnce (Return (1));

  check_fuel command (&mock_obj);
  EXPECT_NO_THROW (command.execute ());
}

TEST(check_fuel_tests, fuel_unsufficiency_test)
{
  mock_fuel mock_obj;
  EXPECT_CALL(mock_obj, get_fuel_reserve ())
          .WillOnce (Return (1));
  EXPECT_CALL(mock_obj, get_burn_fuel_velocity ())
          .WillOnce (Return (10));

  check_fuel command (&mock_obj);
  EXPECT_THROW (command.execute (), command_exception);
}

TEST(check_fuel_tests, incorrect_getters_check_fuel_test)
{
  {
    mock_fuel mock_obj;
    EXPECT_CALL(mock_obj, get_fuel_reserve ())
        .WillOnce (Throw (get_exception ()));

    check_fuel command (&mock_obj);
    EXPECT_THROW (command.execute (), command_exception);
  }

  {
    mock_fuel mock_obj;
    EXPECT_CALL(mock_obj, get_fuel_reserve ())
        .WillOnce (Return (1));
    EXPECT_CALL(mock_obj, get_burn_fuel_velocity ())
        .WillOnce (Throw (get_exception ()));

    check_fuel command (&mock_obj);
    EXPECT_THROW (command.execute (), command_exception);
  }
}

TEST(burn_fuel_tests, correct_burn_fuel_test)
{
  int burn_fuel_velocity = -1;
  mock_fuel mock_obj;
  EXPECT_CALL(mock_obj, get_fuel_reserve ())
          .WillOnce (Return (10));
  EXPECT_CALL(mock_obj, get_burn_fuel_velocity ())
          .WillOnce (Return (1));
  EXPECT_CALL(mock_obj, set_fuel_reserve (_))
          .WillOnce (SaveArg<0> (&burn_fuel_velocity));

  burn_fuel command (&mock_obj);
  command.execute ();

  EXPECT_TRUE (burn_fuel_velocity == 9);
}

TEST(burn_fuel_tests, exceptions_burn_fuel_test)
{
  {
    mock_fuel mock_obj;
    EXPECT_CALL(mock_obj, get_fuel_reserve ())
        .WillOnce (Throw (get_exception ()));

    burn_fuel command (&mock_obj);
    EXPECT_THROW (command.execute (), command_exception);
  }

  {
    mock_fuel mock_obj;
    EXPECT_CALL(mock_obj, get_fuel_reserve ())
        .WillOnce (Return (1));
    EXPECT_CALL(mock_obj, get_burn_fuel_velocity ())
        .WillOnce (Throw (get_exception ()));

    burn_fuel command (&mock_obj);
    EXPECT_THROW (command.execute (), command_exception);
  }

  {
    mock_fuel mock_obj;
    EXPECT_CALL(mock_obj, get_fuel_reserve ())
        .WillOnce (Return (1));
    EXPECT_CALL(mock_obj, get_burn_fuel_velocity ())
        .WillOnce (Return (1));
    EXPECT_CALL(mock_obj, set_fuel_reserve (_))
        .WillOnce (Throw (set_exception ()));

    burn_fuel command_3 (&mock_obj);
    EXPECT_THROW (command_3.execute (), command_exception);
  }
}

