#pragma once

#include "motion.h"
#include "process_fuel_tests.h"
#include "rotation.h"

class mock_move : public move
{
public:
  mock_move () : move (nullptr) {}
  MOCK_METHOD0 (execute, void ());
};

class mock_command : public command
{
public:
  MOCK_METHOD0 (execute, void ());
};

TEST(fuel_burning_move_tests, fuel_sufficiency_test)
{
  int burn_fuel_velocity = -1;
  mock_fuel mock_fuel_obj;
  EXPECT_CALL(mock_fuel_obj, get_fuel_reserve ())
          .WillRepeatedly (Return (10));
  EXPECT_CALL(mock_fuel_obj, get_burn_fuel_velocity ())
          .WillRepeatedly (Return (1));
  EXPECT_CALL(mock_fuel_obj, set_fuel_reserve (_))
          .WillOnce (SaveArg<0> (&burn_fuel_velocity));

  mock_move mock_move_command;
  EXPECT_CALL(mock_move_command, execute ())
          .WillOnce (Return ());

  check_fuel check_fuel_command (&mock_fuel_obj);
  burn_fuel burn_fuel_command (&mock_fuel_obj);

  motion_with_burn_fuel fuel_burning_move_command ({&check_fuel_command, &mock_move_command, &burn_fuel_command});
  EXPECT_NO_THROW (fuel_burning_move_command.execute ());
  EXPECT_TRUE (burn_fuel_velocity == 9);
}

TEST(fuel_burning_move_tests, fuel_unsufficiency_test)
{
  mock_fuel mock_fuel_obj;
  EXPECT_CALL(mock_fuel_obj, get_fuel_reserve ())
          .WillRepeatedly (Return (1));
  EXPECT_CALL(mock_fuel_obj, get_burn_fuel_velocity ())
          .WillRepeatedly (Return (10));

  check_fuel check_fuel_command (&mock_fuel_obj);
  mock_move mock_move_command;
  burn_fuel burn_fuel_command (&mock_fuel_obj);

  motion_with_burn_fuel fuel_burning_move_command ({&check_fuel_command, &mock_move_command, &burn_fuel_command});
  EXPECT_THROW (fuel_burning_move_command.execute (), command_exception);
}

TEST(macrocommand_tests, any_exceptions_test)
{
  mock_command command_1;
  EXPECT_CALL(command_1, execute ())
      .WillOnce (Return ());
  mock_command command_2;
  EXPECT_CALL(command_2, execute ())
          .WillOnce (Throw (command_exception ()));

  macro_command command ({&command_1, &command_2});
  EXPECT_THROW (command.execute (), command_exception);
}

TEST(macrocommand_tests, no_exceptions_test)
{
  mock_command command_1;
  EXPECT_CALL(command_1, execute ())
          .WillOnce (Return ());
  mock_command command_2;
  EXPECT_CALL(command_2, execute ())
          .WillOnce (Return ());

  macro_command command ({&command_1, &command_2});
  EXPECT_NO_THROW (command.execute ());
}
