#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "exceptions.h"
#include "change_velocity_command.h"

using namespace testing;

class mock_movable_and_rotating : public movable_and_rotating_iface
{
public:
  MOCK_CONST_METHOD0 (get_direction, int ());
  MOCK_CONST_METHOD0 (get_direction_count, int ());
  MOCK_CONST_METHOD0 (get_velocity, tuple_2d ());
  MOCK_METHOD1 (set_velocity, void (const tuple_2d &));
};

TEST(change_velocity_tests, correct_calc_velocity)
{
  tuple_2d velocity;
  mock_movable_and_rotating mock_obj;
  EXPECT_CALL(mock_obj, get_direction ())
          .WillRepeatedly (Return (1));
  EXPECT_CALL(mock_obj, get_direction_count ())
          .WillRepeatedly (Return (36));
  EXPECT_CALL(mock_obj, get_velocity ())
          .WillRepeatedly (Return (tuple_2d (10, 10)));
  EXPECT_CALL(mock_obj, set_velocity (_))
          .WillRepeatedly (SaveArg<0> (&velocity));

  change_velocity change_velocity_command (&mock_obj);

  EXPECT_NO_THROW (change_velocity_command.execute ());
  EXPECT_TRUE (velocity.x == 9 && velocity.y == 1);
}


TEST(change_velocity_tests, change_velocity_with_get_exception)
{
  {
    mock_movable_and_rotating mock_obj;
    EXPECT_CALL(mock_obj, get_direction ())
        .WillRepeatedly (Throw (get_exception ()));
    change_velocity change_velocity_command (&mock_obj);
    EXPECT_THROW (change_velocity_command.execute (), command_exception);
  }
  {
    mock_movable_and_rotating mock_obj;
    EXPECT_CALL(mock_obj, get_direction ())
            .WillRepeatedly (Return (1));
    EXPECT_CALL(mock_obj, get_direction_count ())
        .WillRepeatedly (Throw (get_exception ()));
    change_velocity change_velocity_command (&mock_obj);
    EXPECT_THROW (change_velocity_command.execute (), command_exception);
  }
  {
    mock_movable_and_rotating mock_obj;
    EXPECT_CALL(mock_obj, get_direction ())
            .WillRepeatedly (Return (1));
    EXPECT_CALL(mock_obj, get_direction_count ())
            .WillRepeatedly (Return (36));
    EXPECT_CALL(mock_obj, get_velocity ())
        .WillRepeatedly (Throw (get_exception ()));
    change_velocity change_velocity_command (&mock_obj);
    EXPECT_THROW (change_velocity_command.execute (), command_exception);
  }
}

TEST(change_velocity_tests, change_velocity_with_set_exception)
{
  tuple_2d velocity;
  mock_movable_and_rotating mock_obj;
  EXPECT_CALL(mock_obj, get_direction ())
          .WillRepeatedly (Return (1));
  EXPECT_CALL(mock_obj, get_direction_count ())
          .WillRepeatedly (Return (36));
  EXPECT_CALL(mock_obj, get_velocity ())
          .WillRepeatedly (Return (tuple_2d (10, 10)));
  EXPECT_CALL(mock_obj, set_velocity (_))
          .WillRepeatedly (Throw (set_exception ()));

  change_velocity change_velocity_command (&mock_obj);

  EXPECT_THROW (change_velocity_command.execute (), command_exception);
}
