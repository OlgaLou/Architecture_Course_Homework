#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "collision_check.h"

using namespace testing;

class mock_movable : public movable_iface
{
public:
  MOCK_CONST_METHOD0 (get_position, point ());
  MOCK_CONST_METHOD0 (get_velocity, point ());
  MOCK_METHOD1 (set_position, void (const point &));
};

class mock_locality_system : public locality_system
{
public:
  MOCK_CONST_METHOD1 (get_zone_num, int (point));
};

TEST(collision_check_tests, object_change_zone)
{
  mock_locality_system system;
  EXPECT_CALL(system, get_zone_num (_))
          .WillRepeatedly (Return (1));

  mock_movable current_movable, other_movable_1, other_movable_2;
  EXPECT_CALL(current_movable, get_position ())
          .WillRepeatedly (Return (point ()));

  system.add_object (0, &current_movable);
  system.add_object (1, &other_movable_1);
  system.add_object (1, &other_movable_2);

  collision_check_one_to_many collision_check_creator (&current_movable, system);
  collision_check_creator.execute ();

  macro_command *collision_check = collision_checkers::get_checkers (&current_movable, system.get_id ());
  EXPECT_NE (collision_check, nullptr);
  EXPECT_EQ (collision_check->count (), 2);
  EXPECT_THROW (collision_check->execute (), exception_base);
}

